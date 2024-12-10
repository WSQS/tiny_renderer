#include "gl.h"
#include "const.h"
#include "tgaimage.h"


Vec3f barycentric_coord(const Vec3f* pts, const Vec2i p)
{
    Vec3f x(pts[0].x - p.x, pts[1].x - p.x, pts[2].x - p.x), y(pts[0].y - p.y, pts[1].y - p.y, pts[2].y - p.y);
    Vec3f a = x ^ y;
    a = a / a.sum();
    return a;
}

// rasterizer
void triangle(Vec3f* points, float* Z_Buff, TGAImage& image, const Vec3f intensity)
{
    Vec2f Points2d[3], ScreenSize{width / 2, height / 2};
    for (int i = 0; i < 3; i++)
    {
        points[i] = points[i] * points[i].GetPhi();
        Points2d[i] = static_cast<Vec2f>(points[i]);
    }
    // get the bounding box
    const Vec2i right_top{Vec2f::min(Vec2f::max(Points2d[0], Points2d[1], Points2d[2]), ScreenSize)},
                left_bottom{Vec2f::max(Vec2f::min(Points2d[0], Points2d[1], Points2d[2]), ScreenSize * -1.f)};
    for (int x = left_bottom.x; x <= right_top.x; x++)
    {
        for (int y = left_bottom.y; y <= right_top.y; y++)
        {
            // fragment shader
            Vec3f k = barycentric_coord(points, Vec2i(x, y));
            if (k.x < 0 || k.y < 0 || k.z < 0)
                continue;
            const float Z = k * Vec3f(points[0].z, points[1].z, points[2].z);
            const Vec2i p0{Vec2i{x, y} + static_cast<Vec2i>(ScreenSize)};
            float inten = k * intensity;
            inten = std::max(0.0f, inten);
            // std::cout << p0 << k << '\t' << intensity << '\t' << inten << std::endl;
            if (Z >= Z_Buff[p0.x + p0.y * width] && inten >= 0)
            {
                Z_Buff[p0.x + p0.y * width] = Z;
                // std::cout << k << '\t' << intensity << '\t' << inten << std::endl;
                image.set(p0.x, p0.y, TGAColor(static_cast<unsigned char>(255 * inten),
                                               static_cast<unsigned char>(255 * inten),
                                               static_cast<unsigned char>(255 * inten), 255));
            }
        }
    }
}

void triangle(Vec3f* points, IShader* Shader, TGAImage& image, float* Z_Buff)
{
    Vec2f Points2d[3], ScreenSize{width / 2, height / 2};
    for (int i = 0; i < 3; i++)
    {
        points[i] = points[i] * points[i].GetPhi();
        Points2d[i] = static_cast<Vec2f>(points[i]);
    }
    // get the bounding box
    const Vec2i right_top{Vec2f::min(Vec2f::max(Points2d[0], Points2d[1], Points2d[2]), ScreenSize)},
                left_bottom{Vec2f::max(Vec2f::min(Points2d[0], Points2d[1], Points2d[2]), ScreenSize * -1.f)};
    for (int x = left_bottom.x; x <= right_top.x; x++)
    {
        for (int y = left_bottom.y; y <= right_top.y; y++)
        {
            // fragment shader
            Vec3f k = barycentric_coord(points, Vec2i(x, y));
            if (k.x < 0 || k.y < 0 || k.z < 0)
            {
                continue;
            }
            const float Z = k * Vec3f(points[0].z, points[1].z, points[2].z);
            const Vec2i p0{Vec2i{x, y} + static_cast<Vec2i>(ScreenSize)};
            TGAColor Color;
            if (Z >= Z_Buff[p0.x + p0.y * width] && Shader->fragment(k, Color))
            {
                Z_Buff[p0.x + p0.y * width] = Z;
                image.set(p0.x, p0.y, Color);
            }
        }
    }
}

void line(const Vec2i a, const Vec2i b, TGAImage& image, TGAColor color)
{
    bool steep = false;
    int x0 = a.x, y0 = a.y, x1 = b.x, y1 = b.y;
    if (std::abs(x1 - x0) < std::abs(y1 - y0))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    const int dy = y1 - y0;
    if (dx == 0)
    {
        dx = 1;
    }
    for (int x = x0; x <= x1; x++)
    {
        const int y = y0 + dy * (x - x0) / dx;
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
    }
}
