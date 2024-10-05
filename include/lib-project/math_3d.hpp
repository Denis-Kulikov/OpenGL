#pragma once

# define M_PI           3.14159265358979323846

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#include "../include/math/Matrix4f.hpp"

struct vec3i {
    vec3i(int x, int y, int z)
        : x(x), y(y), z(z) {}
        
    int x;
    int y;
    int z;
};
