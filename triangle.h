#ifndef __TRANGLE_H__
#define __TRANGLE_H__
#include "tgaimage.h"
#include "geometry.h"

void triangle_line_sweeping(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);

bool in_triangle(Vec2i t0, Vec2i t1, Vec2i t2, Vec2i p);

#endif