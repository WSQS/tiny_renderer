// Copyright 2025 Sophomore Wang
#pragma once
#include "geometry.h"
#include "tgaimage.h"

const auto white = TGAColor(255, 255, 255, 255);
const auto red = TGAColor(255, 0, 0, 255);
const auto green = TGAColor(0, 255, 0, 255);
constexpr int width = 800;
constexpr int height = 800;
constexpr int depth = 255;
constexpr Vec3f eye = Vec3f({1, 1, 2}) * 2, CenterOfScreen{0, 0, 0};

inline float GetC() { return (eye - CenterOfScreen) * (eye - CenterOfScreen) * depth; }

constexpr Matrix<float, 4, 4> model_view() {
    constexpr Vec3f up{0, 1, 0};
    Vec3f xyz[3];
    xyz[2] = (eye - CenterOfScreen).normalize();
    xyz[0] = (up ^ xyz[2]).normalize();
    xyz[1] = (xyz[0] ^ xyz[2]).normalize();
    Matrix<float, 4, 4> Minv{};
    Matrix<float, 4, 4> Tr{};
    for (int i = 0; i < 3; i++) {
        Minv.get(0, i) = xyz[0].get(i, 0);
        Minv.get(1, i) = xyz[1].get(i, 0);
        Minv.get(2, i) = xyz[2].get(i, 0);
        Tr.get(i, 3) = -CenterOfScreen.get(i, 0);
    }
    Minv.get(3, 3) = 1;
    Tr.get(0, 0) = 1;
    Tr.get(1, 1) = 1;
    Tr.get(2, 2) = 1;
    Tr.get(3, 3) = 1;
    return Minv * Tr;
}
constexpr Matrix<float, 4, 4> projection() {
    return {{1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}, {0.f, 0.f, 1.f, 1.f}, {0.f, 0.f, 0.f, 1.f}};
}

inline float GetPhi(Vec3f v) { return 1.f / (1.f - v.data[2][0] / GetC()); }
