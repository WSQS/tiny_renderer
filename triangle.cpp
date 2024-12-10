
#include "tgaimage.h"
#include "geometry.h"
float c = 10;
int width = 800;
int height = 800;

Vec3f barycentric_coord1(Vec3f *pts, Vec2i p)
{
    Vec3f x(pts[0].x - p.x, pts[1].x - p.x, pts[2].x - p.x), y(pts[0].y - p.y, pts[1].y - p.y, pts[2].y - p.y);
    Vec3f a = x ^ y;
    a.normalize();
    return a;
}
void triangle_line_sweeping(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    // sort three points by y order
    if (t0.y > t1.y)
        std::swap(t0, t1);
    if (t1.y > t2.y)
        std::swap(t1, t2);
    if (t0.y > t1.y)
        std::swap(t0, t1);
    Vec2i d1 = t1 - t0, d2 = t2 - t0, d3 = t2 - t1;
    for (int y = t0.y; y <= t1.y; y++)
    {
        if (d1.y == 0)
        {
            int x0 = t0.x, x1 = t1.x;
            if (x0 > x1)
                std::swap(t0, t1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
        else
        {
            int x0 = t0.x + d1.x * (y - t0.y) / d1.y;
            int x1 = t0.x + d2.x * (y - t0.y) / d2.y;
            if (x0 > x1)
                std::swap(x0, x1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
    }
    for (int y = t1.y; y <= t2.y; y++)
    {
        if (d3.y == 0)
        {
            int x0 = t1.x, x1 = t2.x;
            if (x0 > x1)
                std::swap(t0, t1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
        else
        {
            int x0 = t1.x + d3.x * (y - t1.y) / d3.y;
            int x1 = t0.x + d2.x * (y - t0.y) / d2.y;
            if (x0 > x1)
                std::swap(x0, x1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
    }
}

bool in_triangle(Vec2i t0, Vec2i t1, Vec2i t2, Vec2i p)
{
    Vec3i x(t0.x - p.x, t1.x - p.x, t2.x - p.x), y(t0.y - p.y, t1.y - p.y, t2.y - p.y);
    Vec3i a = x ^ y;
    return (a.x >= 0 && a.y >= 0 && a.z >= 0) || (a.x <= 0 && a.y <= 0 && a.z <= 0);
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    // get the bounding box
    Vec2i right_top(std::max(std::max(t0.x, t1.x), t2.x), std::max(std::max(t0.y, t1.y), t2.y)),
        left_bottom(std::min(std::min(t0.x, t1.x), t2.x), std::min(std::min(t0.y, t1.y), t2.y));
    for (int x = left_bottom.x; x <= right_top.x; x++)
    {
        for (int y = left_bottom.y; y <= right_top.y; y++)
        {
            Vec2i p(x, y);
            if (in_triangle(t0, t1, t2, p))
                image.set(x, y, color);
        }
    }
}

void triangle_zbuff(Vec3f *points, float *zbuff, TGAImage &image, TGAColor color)
{
    // get the bounding box
    for (int i = 0; i < 3; i++)
    {
        float phi = 1. / (1. - points[i].z / c);
        points[i] = points[i] * phi;
    }
    Vec2i right_top(std::min(std::max(std::max(points[0].x, points[1].x), points[2].x), float(width / 2)), std::min(std::max(std::max(points[0].y, points[1].y), points[2].y), float(height / 2))),
        left_bottom(std::max(std::min(std::min(points[0].x, points[1].x), points[2].x), -float(width / 2)), std::max(std::min(std::min(points[0].y, points[1].y), points[2].y), -float(height / 2)));
    Vec3f p;
    for (int x = left_bottom.x; x <= right_top.x; x++)
    {
        for (int y = left_bottom.y; y <= right_top.y; y++)
        {
            Vec3f k = barycentric_coord1(points, Vec2i(x, y));
            if (k.x < 0 || k.y < 0 || k.z < 0)
                continue;
            p = Vec3f(x, y, k * Vec3f(points[0].z, points[1].z, points[2].z));
            Vec2i p0(p.x + width / 2, p.y + height / 2);
            if (p.z >= zbuff[p0.x + p0.y * width])
            {
                zbuff[p0.x + p0.y * width] = p.z;
                image.set(p0.x, p0.y, color);
            }
        }
    }
}