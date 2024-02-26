#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    const Matrix4f* GetTrans();

    Camera *camera;
    objectTransform *object;
    Matrix4f m_transformation;
};
