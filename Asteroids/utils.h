// utils.h
#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

namespace Utils {
    float randomFloat(float min, float max);
    float VectorDistance(const Vector2& v1, const Vector2& v2);
    Vector2 VectorLerp(const Vector2& v1, const Vector2& v2, float t);
}

#endif