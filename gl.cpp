// Copyright 2025 Sophomore Wang
#include "gl.h"

#include "const.h"
#include "tgaimage.h"

#include <memory>

Vec3f barycentric_coord(const Vec3f *pts, const Vec2i p) {
    Vec3f x{pts[0].get(0, 0) - p.get(0, 0), pts[1].get(0, 0) - p.get(0, 0),
        pts[2].get(0, 0) - p.get(0, 0)},
        y{pts[0].get(1, 0) - p.get(1, 0), pts[1].get(1, 0) - p.get(1, 0),
            pts[2].get(1, 0) - p.get(1, 0)};
    Vec3f a = x ^ y;
    a = a / a.sum();
    return a;
}

// rasterizer
void triangle(Vec3f *points, float *Z_Buff, TGAImage &image, const Vec3f intensity) {
    Vec2f Points2d[3], ScreenSize{width / 2, height / 2};
    for (int i = 0; i < 3; i++) {
        points[i] = points[i] * GetPhi(points[i]);
        Points2d[i] = static_cast<Vec2f>(points[i]);
    }
    // get the bounding box
    const Vec2i right_top{
        Vec2f::min(Vec2f::max(Points2d[0], Points2d[1], Points2d[2]), ScreenSize)},
        left_bottom{
            Vec2f::max(Vec2f::min(Points2d[0], Points2d[1], Points2d[2]), ScreenSize * -1.f)};
    for (int x = left_bottom.get(0, 0); x <= right_top.get(0, 0); x++) {
        for (int y = left_bottom.get(1, 0); y <= right_top.get(1, 0); y++) {
            // fragment shader
            Vec3f k = barycentric_coord(points, Vec2i{x, y});
            if (k.get(0, 0) < 0 || k.get(1, 0) < 0 || k.get(2, 0) < 0)
                continue;
            const float Z =
                k * Vec3f{points[0].get(2, 0), points[1].get(2, 0), points[2].get(2, 0)};
            const Vec2i p0{Vec2i{x, y} + static_cast<Vec2i>(ScreenSize)};
            float inten = k * intensity;
            inten = std::max(0.0f, inten);
            // std::cout << p0 << k << '\t' << intensity << '\t' << inten << std::endl;
            if (Z >= Z_Buff[p0.get(0, 0) + p0.get(1, 0) * width] && inten >= 0) {
                Z_Buff[p0.get(0, 0) + p0.get(1, 0) * width] = Z;
                // std::cout << k << '\t' << intensity << '\t' << inten << std::endl;
                image.set(p0.get(0, 0), p0.get(1, 0),
                    TGAColor(static_cast<unsigned char>(255 * inten),
                        static_cast<unsigned char>(255 * inten),
                        static_cast<unsigned char>(255 * inten), 255));
            }
        }
    }
}

void triangle(Vec3f *points, std::shared_ptr<IShader> Shader, TGAImage &image, float *Z_Buff) {
    Vec2f Points2d[3], ScreenSize{width / 2, height / 2};
    for (int i = 0; i < 3; i++) {
        points[i] = points[i] * GetPhi(points[i]);
        Points2d[i] = static_cast<Vec2f>(points[i]);
    }
    // get the bounding box
    const Vec2i right_top{
        Vec2f::min(Vec2f::max(Points2d[0], Points2d[1], Points2d[2]), ScreenSize)},
        left_bottom{
            Vec2f::max(Vec2f::min(Points2d[0], Points2d[1], Points2d[2]), ScreenSize * -1.f)};
    for (int x = left_bottom.get(0, 0); x <= right_top.get(0, 0); x++) {
        for (int y = left_bottom.get(1, 0); y <= right_top.get(1, 0); y++) {
            // fragment shader
            Vec3f k = barycentric_coord(points, Vec2i{x, y});
            if (k.get(0, 0) < 0 || k.get(1, 0) < 0 || k.get(2, 0) < 0)
                continue;
            const float Z =
                k * Vec3f{points[0].get(2, 0), points[1].get(2, 0), points[2].get(2, 0)};
            const Vec2i p0{Vec2i{x, y} + static_cast<Vec2i>(ScreenSize)};
            TGAColor Color;
            if (Z >= Z_Buff[p0.get(0, 0) + p0.get(1, 0) * width] && Shader->fragment(k, Color)) {
                Z_Buff[p0.get(0, 0) + p0.get(1, 0) * width] = Z;
                image.set(p0.get(0, 0), p0.get(1, 0), Color);
            }
        }
    }
}

void line(const Vec2i a, const Vec2i b, TGAImage &image, TGAColor color) {
    bool steep = false;
    int x0 = a.get(0, 0), y0 = a.get(1, 0), x1 = b.get(0, 0), y1 = b.get(1, 0);
    if (std::abs(x1 - x0) < std::abs(y1 - y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    const int dy = y1 - y0;
    if (dx == 0)
        dx = 1;
    for (int x = x0; x <= x1; x++) {
        const int y = y0 + dy * (x - x0) / dx;
        if (steep)
            image.set(y, x, color);
        else
            image.set(x, y, color);
    }
}
