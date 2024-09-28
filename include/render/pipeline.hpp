#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f GetTransform(const objectTransform& transform) const;
    Matrix4f GetWorld(const objectTransform& transform) const;

    Camera *camera;
};
