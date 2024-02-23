#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    const Matrix4f<GLfloat>* GetTrans();

    Camera camera;
    objectTransform object;
    Matrix4f<GLfloat> m_transformation;
};
