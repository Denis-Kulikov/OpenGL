#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f<GLfloat> GetTransform(const objectTransform& transform) const;
    // GLfloat* GetGLMatrix(const objectTransform *transform) const;

    Camera *camera;
};
