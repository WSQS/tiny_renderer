#pragma once

#include "geometry.h"
#include "tgaimage.h"

#include <memory>

class IShader {
public:
    virtual ~IShader() = default;
    /// @brief vertex shader
    /// @param iface face index
    /// @param nthvert vertex index in face
    /// @return vertex screen coordinate
    virtual Vec3i vertex(int iface, int nthvert) = 0;
    /// @brief fragment shader
    /// @param bar the coordinate of the point
    /// @param color the color of point
    /// @return should the point be rendered
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

void triangle(Vec3f *points, float *Z_Buff, TGAImage &image, Vec3f intensity);
void triangle(Vec3f *points, std::shared_ptr<IShader> Shader, TGAImage &image, float *Z_Buff);
void line(Vec2i a, Vec2i b, TGAImage &image, TGAColor color);
