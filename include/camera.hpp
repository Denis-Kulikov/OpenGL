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

    void SetPerspectiveProj(const Camera &_camera)
    {
        PersProj.FOV    = _camera.PersProj.FOV;
        PersProj.Width  = _camera.PersProj.Width;
        PersProj.Height = _camera.PersProj.Height;
        PersProj.zNear  = _camera.PersProj.zNear;
        PersProj.zFar   = _camera.PersProj.zFar;
    }

    void SetCamera(const Vector3<GLfloat>& WorldPos, const Vector3<GLfloat>& Target, const Vector3<GLfloat>& Up)
    {
        Params.WorldPos = WorldPos;
        Params.Target   = Target;
        Params.Up       = Up;
    }

    void SetCamera(const Camera &_camera)
    {
        SetCamera(_camera.Params.WorldPos, _camera.Params.Target, _camera.Params.Up);
        SetPerspectiveProj(_camera.PersProj.FOV, _camera.PersProj.Width, _camera.PersProj.Height, _camera.PersProj.zNear, _camera.PersProj.zFar);
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
