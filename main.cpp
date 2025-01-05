#include <cmath>
#include <memory>
#include <vector>
// #include <iostream>
#include "const.h"
#include "geometry.h"
#include "gl.h"
#include "model.h"
#include "tgaimage.h"

Model *model;
const Vec3f light_dir = Vec3f{1, 0, 1}.normalize(); // define light_direction

class GouraudShader : public IShader {
    Vec3f VaryingIntensity; // written by vertex shader, read by fragment shader
    Vec3f VaryingU;
    Vec3f VaryingV;

public:
    Vec3i vertex(int iface, int nthvert) override {
        VaryingIntensity.get(nthvert, 0) = std::max(0.f, model->normal(iface, nthvert) * light_dir);
        VaryingU.get(nthvert, 0) = static_cast<float>(model->uv(iface, nthvert).get(0, 0));
        VaryingV.get(nthvert, 0) = static_cast<float>(model->uv(iface, nthvert).get(1, 0));
        // get diffuse lighting intensity
        Vec3f gl_Vertex{model->vert(iface, nthvert)}; // read the vertex from .obj file
        // TODO:待矩阵化
        // gl_Vertex = BuildAxis() * (gl_Vertex - CenterOfScreen);
        // gl_Vertex = (gl_Vertex + Vec3f{0, 0, 1.f}) / 2.f;
        gl_Vertex = projection() * model_view() * (gl_Vertex << 1.f) / 2;
        // gl_Vertex = Vec3f::ParallelDot(gl_Vertex, Vec3f{width, height, depth});
        gl_Vertex = Matrix<float, 4, 4>{{width, 0.f, 0.f, 0.f}, {0.f, height, 0.f, 0.f},
                        {0.f, 0.f, depth, 0.f}, {0.f, 0.f, 0.f, 1.f}} *
                    (gl_Vertex << 1.f);
        gl_Vertex = gl_Vertex * GetPhi(gl_Vertex);
        return static_cast<Vec3i>(gl_Vertex); // transform it to screen coordinates
    }

    bool fragment(Vec3f bar, TGAColor &color) override {
        float diff = VaryingIntensity * bar; // interpolate intensity for the current pixel
        Vec2f uv{VaryingU * bar, VaryingV * bar};
        Vec3f n = model->normal(uv);
        n = n.normalize();
        Vec3f l = light_dir;
        Vec3f r = (n * (n * l * 2.f) - l).normalize(); // reflected light
        float spec = pow(std::max(r.get(2, 0), 0.0f), model->specular(uv)/5);
        diff = std::max(0.f, n * l);
        if (diff < 0.f)
            return false;
        for (int i = 0; i < 3; i++) {
            color.bgra[i] =
                std::min<int>(5 + model->diffuse(uv).bgra[i] * (diff + spec * 0.6), 255);
        }
        // color = TGAColor(255, 255, 255) * intensity; // well duh
        return true; // no, we do not discard this pixel
    }
};

int main(const int argc, char **argv) {
    // check for argument
    if (2 == argc)
        model = new Model(argv[1]);
    else
        model = new Model("obj/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);

    Matrix ModelView(model_view());
    // std::cout << M << std::endl;
    // initialize Z Buffer
    const auto Z_Buffer = new float[width * height];
    for (int i = 0; i < width * height; i++)
        Z_Buffer[i] = std::numeric_limits<float>::min();
    auto shader = std::make_shared<GouraudShader>();
    for (int i = 0; i < model->nfaces(); i++) {
        Vec3f screen_coords[3];
        for (int j = 0; j < 3; j++)
            screen_coords[j] = static_cast<Vec3f>(shader->vertex(i, j));
        triangle(screen_coords, shader, image, Z_Buffer);
    }
    // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
