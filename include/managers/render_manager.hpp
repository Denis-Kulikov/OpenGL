#pragma once
#include "../render/render.hpp"

class RenderManager  
{
public:
    static void Initialize(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    static void Dispose();

    static void PushCamera(const Camera &_camera);
    static void UpdateCamera();

    inline static Render render;
};
