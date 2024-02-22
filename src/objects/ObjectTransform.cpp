#include "../include/glfw.hpp"

Vector3f ObjectTransform::GetScale()
{
    return Scale;
}

Vector3f ObjectTransform::GetWorldPos()
{
    return WorldPos;
}

Vector3f ObjectTransform::GetRotate()
{
    return Rotate;
}


void ObjectTransform::SetScale(double ScaleX, double ScaleY, double ScaleZ)
{
    Scale.x = ScaleX;
    Scale.y = ScaleY;
    Scale.z = ScaleZ;
}

void ObjectTransform::SetScale(Vector3f v_Scale)
{
    Scale.x = v_Scale.x;
    Scale.y = v_Scale.y;
    Scale.z = v_Scale.z;
}


void ObjectTransform::SetWorldPos(double WorldPosX, double WorldPosY, double WorldPosZ)
{
    WorldPos.x = WorldPosX;
    WorldPos.y = WorldPosY;
    WorldPos.z = WorldPosZ;
}

void ObjectTransform::SetWorldPos(Vector3f v_WorldPos)
{
    WorldPos.x = v_WorldPos.x;
    WorldPos.y = v_WorldPos.y;
    WorldPos.z = v_WorldPos.z;
}


void ObjectTransform::SetRotate(double RotateX, double RotateY, double RotateZ)
{
    Rotate.x = RotateX;
    Rotate.y = RotateY;
    Rotate.z = RotateZ;
}

void ObjectTransform::SetRotate(Vector3f v_Rotate)
{
    Rotate.x = v_Rotate.x;
    Rotate.y = v_Rotate.y;
    Rotate.z = v_Rotate.z;
}


void ObjectTransform::SetTransform(ObjectTransform trans)
{
    SetScale(trans.GetScale());
    SetRotate(trans.GetRotate());
    SetWorldPos(trans.GetWorldPos());
}
