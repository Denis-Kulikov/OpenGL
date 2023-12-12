#pragma once
#include "camera.hpp"
#include "ObjectTransform.hpp"

class Pipeline
{
public:
    Pipeline() {}

    const Matrix4f* GetTrans();

    Camera camera;
    ObjectTransform object;

    Matrix4f m_transformation;
};
