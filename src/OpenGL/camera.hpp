#pragma once
#include "math_3d.h"

class Camera
{
public:
    Camera()
    {
        Params.WorldPos    = Vector3f(0.0f, 0.0f, 0.0f);
        Params.Target      = Vector3f(0.0f, 0.0f, 1.0f);
        Params.Up          = Vector3f(0.0f, 1.0f, 0.0f);
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        PersProj.FOV    = FOV;
        PersProj.Width  = Width;
        PersProj.Height = Height;
        PersProj.zNear  = zNear;
        PersProj.zFar   = zFar;
    }

    void SetCamera(const Vector3f& WorldPos, const Vector3f& Target, const Vector3f& Up)
    {
        Params.WorldPos = WorldPos;
        Params.Target = Target;
        Params.Up = Up;
    }

    struct {
        Vector3f WorldPos;
        Vector3f Target;
        Vector3f Up;
    } Params;

    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } PersProj;
};
