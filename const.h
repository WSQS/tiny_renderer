#pragma once
#include "geometry.h"
#include "tgaimage.h"

const auto white = TGAColor(255, 255, 255, 255);
const auto red = TGAColor(255, 0, 0, 255);
const auto green = TGAColor(0, 255, 0, 255);
constexpr int width = 800;
constexpr int height = 800;
constexpr int depth = 255;
const Vec3f eye = Vec3f({1, 1, 2}) * 2, CenterOfScreen{0, 0, 0};

inline float GetC()
{
    return (eye - CenterOfScreen) * (eye - CenterOfScreen) * depth;
}

inline Matrix<float,3,3> BuildAxis()
{
    static Vec3f u{0, 1, 0};
    Vec3f xyz[3];
    xyz[2] = (eye - CenterOfScreen).normalize();
    xyz[0] = (u ^ xyz[2]).normalize();
    xyz[1] = (xyz[0] ^ xyz[2]).normalize();
    return Matrix<float,3,3>::merge({xyz[0], xyz[1], xyz[2]});
}

inline float GetPhi(Vec3f v) {
    return 1.f/(1.f-v.data[2][0]/GetC());
}

