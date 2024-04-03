#pragma once
#include "camera.hpp"

class Pipeline
{
public:
    Pipeline() {}

    Matrix4f<GLfloat>* GetTransform();
    GLfloat* GetGLMatrix();

    Camera *camera;
    objectTransform *object;
    Matrix4f<GLfloat> m_transformation;
};
