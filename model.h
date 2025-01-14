// Copyright 2025 Sophomore Wang
#pragma once
#include "geometry.h"
#include "tgaimage.h"
#include <vector>

class Model {
private:
    std::vector<Vec3f> verts_{};
    std::vector<std::vector<Vec3i>> faces_{}; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_{};
    std::vector<Vec2f> uv_{};
    TGAImage diffusemap_{};
    TGAImage normalmap_{};
    TGAImage specularmap_{};
    void load_texture(std::string filename, const char *suffix, TGAImage &img);

public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f normal(int iface, int nvert);
    Vec3f normal(Vec2f uvf);
    float specular(Vec2f uvf);
    Vec3f vert(int i);
    Vec3f vert(int iface, int nvert);
    Vec2f uv(int iface, int nvert);
    TGAColor diffuse(Vec2f uv);
    // Get face from index
    std::vector<int> face(int idx);
};
