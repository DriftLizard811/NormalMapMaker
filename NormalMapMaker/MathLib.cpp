#include "MathLib.h"

int clamp(int v, int l, int h)
{
    return std::max(std::min(v, h), l);
}

float lerp(float a, float b, float t)
{
    return a + ((b - a) * t);
}