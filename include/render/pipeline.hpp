#pragma once
#include "camera.hpp"
#include "math/Matrix4f.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f GetTransform(const objectTransform& transform) const;
    Matrix4f GetWorld(const objectTransform& transform) const;

    Camera camera;
};
