#pragma once
#include "math_3d.h"

class ObjectTransform 
{
public:
    ObjectTransform()
    {
        Scale = Vector3f(1.0f, 1.0f, 1.0f);
        WorldPos = Vector3f(0.0f, 0.0f, 0.0f);
        Rotate = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void SetScale(float ScaleX, float ScaleY, float ScaleZ)
    {
        Scale.x = ScaleX;
        Scale.y = ScaleY;
        Scale.z = ScaleZ;
    }

    void SetWorldPos(float x, float y, float z)
    {
        WorldPos.x = x;
        WorldPos.y = y;
        WorldPos.z = z;
    }

    void SetRotate(float RotateX, float RotateY, float RotateZ)
    {
        Rotate.x = RotateX;
        Rotate.y = RotateY;
        Rotate.z = RotateZ;
    }

    Vector3f Scale;
    Vector3f WorldPos;
    Vector3f Rotate;
};
