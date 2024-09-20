#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f GetTransform(const objectTransform& transform) const;

    Camera *camera;
};
