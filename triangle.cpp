
#include "geometry.h"
#include "tgaimage.h"
float c = 10;
int width = 800;
int height = 800;

Vec3f barycentric_coord1(Vec3f *pts, Vec2i p) {
    Vec3f x{pts[0].get(0, 0) - p.get(0, 0), pts[1].get(0, 0) - p.get(0, 0),
            pts[2].get(0, 0) - p.get(0, 0)},
        y{pts[0].get(1, 0) - p.get(1, 0), pts[1].get(1, 0) - p.get(1, 0),
          pts[2].get(1, 0) - p.get(1, 0)};
    Vec3f a = x ^ y;
    a.normalize();
    return a;
}
void triangle_line_sweeping(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // sort three points by y order
    if (t0.get(1, 0) > t1.get(1, 0))
        std::swap(t0, t1);
    if (t1.get(1, 0) > t2.get(1, 0))
        std::swap(t1, t2);
    if (t0.get(1, 0) > t1.get(1, 0))
        std::swap(t0, t1);
    Vec2i d1 = t1 - t0, d2 = t2 - t0, d3 = t2 - t1;
    for (int y = t0.get(1, 0); y <= t1.get(1, 0); y++) {
        if (d1.get(1, 0) == 0) {
            int x0 = t0.get(0, 0), x1 = t1.get(0, 0);
            if (x0 > x1)
                std::swap(t0, t1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        } else {
            int x0 = t0.get(0, 0) + d1.get(0, 0) * (y - t0.get(1, 0)) / d1.get(1, 0);
            int x1 = t0.get(0, 0) + d2.get(0, 0) * (y - t0.get(1, 0)) / d2.get(1, 0);
            if (x0 > x1)
                std::swap(x0, x1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
    }
    for (int y = t1.get(1, 0); y <= t2.get(1, 0); y++) {
        if (d3.get(1, 0) == 0) {
            int x0 = t1.get(0, 0), x1 = t2.get(0, 0);
            if (x0 > x1)
                std::swap(t0, t1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        } else {
            int x0 = t1.get(0, 0) + d3.get(0, 0) * (y - t1.get(1, 0)) / d3.get(1, 0);
            int x1 = t0.get(0, 0) + d2.get(0, 0) * (y - t0.get(1, 0)) / d2.get(1, 0);
            if (x0 > x1)
                std::swap(x0, x1);
            for (int x = x0; x <= x1; x++)
                image.set(x, y, color);
        }
    }
}

bool in_triangle(Vec2i t0, Vec2i t1, Vec2i t2, Vec2i p) {
    Vec3i x{t0.get(0, 0) - p.get(0, 0), t1.get(0, 0) - p.get(0, 0), t2.get(0, 0) - p.get(0, 0)},
        y{t0.get(1, 0) - p.get(1, 0), t1.get(1, 0) - p.get(1, 0), t2.get(1, 0) - p.get(1, 0)};
    Vec3i a = x ^ y;
    return (a.get(0, 0) >= 0 && a.get(1, 0) >= 0 && a.get(2, 0) >= 0) ||
           (a.get(0, 0) <= 0 && a.get(1, 0) <= 0 && a.get(2, 0) <= 0);
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // get the bounding box
    Vec2i right_top{std::max(std::max(t0.get(0, 0), t1.get(0, 0)), t2.get(0, 0)),
                    std::max(std::max(t0.get(1, 0), t1.get(1, 0)), t2.get(1, 0))},
        left_bottom{std::min(std::min(t0.get(0, 0), t1.get(0, 0)), t2.get(0, 0)),
                    std::min(std::min(t0.get(1, 0), t1.get(1, 0)), t2.get(1, 0))};
    for (int x = left_bottom.get(0, 0); x <= right_top.get(0, 0); x++) {
        for (int y = left_bottom.get(1, 0); y <= right_top.get(1, 0); y++) {
            Vec2i p{x, y};
            if (in_triangle(t0, t1, t2, p))
                image.set(x, y, color);
        }
    }
}

void triangle_zbuff(Vec3f *points, float *zbuff, TGAImage &image, TGAColor color) {
    // get the bounding box
    for (int i = 0; i < 3; i++) {
        float phi = 1. / (1. - points[i].get(2, 0) / c);
        points[i] = points[i] * phi;
    }
    Vec2i right_top{
        static_cast<int>(std::min(
            std::max(std::max(points[0].get(0, 0), points[1].get(0, 0)), points[2].get(0, 0)),
            float(width / 2))),
        static_cast<int>(std::min(
            std::max(std::max(points[0].get(1, 0), points[1].get(1, 0)), points[2].get(1, 0)),
            float(height / 2)))},
        left_bottom{
            static_cast<int>(std::max(
                std::min(std::min(points[0].get(0, 0), points[1].get(0, 0)), points[2].get(0, 0)),
                -float(width / 2))),
            static_cast<int>(std::max(
                std::min(std::min(points[0].get(1, 0), points[1].get(1, 0)), points[2].get(1, 0)),
                -float(height / 2)))};
    Vec3f p;
    for (int x = left_bottom.get(0, 0); x <= right_top.get(0, 0); x++) {
        for (int y = left_bottom.get(1, 0); y <= right_top.get(1, 0); y++) {
            Vec3f k = barycentric_coord1(points, Vec2i{x, y});
            if (k.get(0, 0) < 0 || k.get(1, 0) < 0 || k.get(2, 0) < 0)
                continue;
            p = Vec3f{
                static_cast<float>(x),
                static_cast<float>(y),
                k * Vec3f{points[0].get(2, 0), points[1].get(2, 0), points[2].get(2, 0)}};
            Vec2i p0{static_cast<int>(p.get(0, 0)) + width / 2,
                     static_cast<int>(p.get(1, 0)) + height / 2};
            if (p.get(2, 0) >= zbuff[p0.get(0, 0) + p0.get(1, 0) * width]) {
                zbuff[p0.get(0, 0) + p0.get(1, 0) * width] = p.get(2, 0);
                image.set(p0.get(0, 0), p0.get(1, 0), color);
            }
        }
    }
}
