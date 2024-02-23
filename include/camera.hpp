#pragma once
#include "objectTransform.hpp"

class Camera
{
public:
    Camera()
    {
        Params.WorldPos    = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
        Params.Target      = Vector3<GLfloat>(0.0f, 0.0f, 1.0f);
        Params.Up          = Vector3<GLfloat>(0.0f, 1.0f, 0.0f);
    }

    void SetPerspectiveProj(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
    {
        PersProj.FOV    = FOV;
        PersProj.Width  = Width;
        PersProj.Height = Height;
        PersProj.zNear  = zNear;
        PersProj.zFar   = zFar;
    }

    void SetCamera(const Vector3<GLfloat>& WorldPos, const Vector3<GLfloat>& Target, const Vector3<GLfloat>& Up)
    {
        Params.WorldPos = WorldPos;
        Params.Target = Target;
        Params.Up = Up;
    }

    struct {
        Vector3<GLfloat> WorldPos;
        Vector3<GLfloat> Target;
        Vector3<GLfloat> Up;
    } Params;

    struct {
        GLfloat FOV;
        GLfloat Width;
        GLfloat Height;
        GLfloat zNear;
        GLfloat zFar;
    } PersProj;
};
