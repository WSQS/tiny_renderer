#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v.get(i, 0);
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i = 0; i < 3; i++)
                iss >> n.get(i, 0);
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++)
                iss >> uv.get(i, 0);
            uv_.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp.get(0, 0) >> trash >> tmp.get(1, 0) >> trash >> tmp.get(2, 0)) {
                for (int i = 0; i < 3; i++)
                    tmp.get(i, 0)--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size()
              << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm_tangent.tga", normalmap_);
    load_texture(filename, "_spec.tga", specularmap_);
}

Model::~Model() {}

int Model::nverts() { return (int)verts_.size(); }

int Model::nfaces() { return (int)faces_.size(); }

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++)
        face.push_back(faces_[idx][i].get(0, 0));
    return face;
}

Vec3f Model::vert(int i) { return verts_[i]; }

Vec3f Model::vert(int iface, int nvert) { return verts_[faces_[iface][nvert].get(0, 0)]; }

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading "
                  << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2f uv) {
    return diffusemap_.get(
        uv.get(0, 0) * diffusemap_.get_width(), uv.get(1, 0) * diffusemap_.get_height());
}

Vec2f Model::uv(int iface, int nvert) { return uv_[faces_[iface][nvert].get(1, 0)]; }

Vec3f Model::normal(int iface, int nvert) {
    int idx = faces_[iface][nvert].get(2, 0);
    return norms_[idx].normalize();
}

Vec3f Model::normal(Vec2f uvf) {
    Vec2i uv{static_cast<int>(uvf.get(0, 0) * normalmap_.get_width()),
        static_cast<int>(uvf.get(1, 0) * normalmap_.get_height())};
    TGAColor c = normalmap_.get(uv.get(0, 0), uv.get(1, 0));
    Vec3f res;
    for (int i = 0; i < 3; i++)
        res.get(2 - i, 0) = (float)c.bgra[i] / 255.f * 2.f - 1.f;
    return res;
}

float Model::specular(Vec2f uvf) {
    Vec2i uv{static_cast<int>(uvf.get(0, 0)) * specularmap_.get_width(),
        static_cast<int>(uvf.get(1, 0)) * specularmap_.get_height()};
    return specularmap_.get(uv.get(0, 0), uv.get(1, 0)).bgra[0] / 1.f;
}
