// utils.cpp
#include "utils.h"
#include <cmath>
#include <random>

float Utils::randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

float Utils::VectorDistance(const Vector2& v1, const Vector2& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return sqrtf(dx * dx + dy * dy);
}

Vector2 Utils::VectorLerp(const Vector2& v1, const Vector2& v2, float t) {
    return {
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t
    };
}