#include "../include/glfw.hpp"

Vector3<GLfloat> objectTransform::GetScale()
{
    return Scale;
}

Vector3<GLfloat> objectTransform::GetWorldPos()
{
    return WorldPos;
}

Vector3<GLfloat> objectTransform::GetRotate()
{
    return Rotate;
}


void objectTransform::SetScale(GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ)
{
    Scale.x = ScaleX;
    Scale.y = ScaleY;
    Scale.z = ScaleZ;
}

void objectTransform::SetScale(Vector3<GLfloat> v_Scale)
{
    Scale.x = v_Scale.x;
    Scale.y = v_Scale.y;
    Scale.z = v_Scale.z;
}


void objectTransform::SetWorldPos(GLfloat WorldPosX, GLfloat WorldPosY, GLfloat WorldPosZ)
{
    WorldPos.x = WorldPosX;
    WorldPos.y = WorldPosY;
    WorldPos.z = WorldPosZ;
}

void objectTransform::SetWorldPos(Vector3<GLfloat> v_WorldPos)
{
    WorldPos.x = v_WorldPos.x;
    WorldPos.y = v_WorldPos.y;
    WorldPos.z = v_WorldPos.z;
}


void objectTransform::SetRotate(GLfloat RotateX, GLfloat RotateY, GLfloat RotateZ)
{
    Rotate.x = RotateX;
    Rotate.y = RotateY;
    Rotate.z = RotateZ;
}

void objectTransform::SetRotate(Vector3<GLfloat> v_Rotate)
{
    Rotate.x = v_Rotate.x;
    Rotate.y = v_Rotate.y;
    Rotate.z = v_Rotate.z;
}


void objectTransform::SetTransform(objectTransform trans)
{
    SetScale(trans.GetScale());
    SetRotate(trans.GetRotate());
    SetWorldPos(trans.GetWorldPos());
}
