#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f<GLfloat>* GetTrans();
    GLfloat* GetGLMatrix();

    Camera *camera;
    objectTransform *object;
    Matrix4f<GLfloat> m_transformation;
};
