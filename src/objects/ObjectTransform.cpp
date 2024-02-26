#include "../include/glfw.hpp"

Vector3<GLfloat> objectTransform::GetScale() const
{
    return Scale;
}

Vector3<GLfloat> objectTransform::GetWorldPos() const
{
    return WorldPos;
}

Vector3<GLfloat> objectTransform::GetRotate() const
{
    return Rotate;
}


void objectTransform::SetScale(const GLfloat &ScaleX, const GLfloat &ScaleY, const GLfloat &ScaleZ)
{
    Scale.x = ScaleX;
    Scale.y = ScaleY;
    Scale.z = ScaleZ;
}

void objectTransform::SetScale(const Vector3<GLfloat> &v_Scale)
{
    Scale.x = v_Scale.x;
    Scale.y = v_Scale.y;
    Scale.z = v_Scale.z;
}


void objectTransform::SetWorldPos(const GLfloat &WorldPosX, const GLfloat &WorldPosY, const GLfloat &WorldPosZ)
{
    WorldPos.x = WorldPosX;
    WorldPos.y = WorldPosY;
    WorldPos.z = WorldPosZ;
}

void objectTransform::SetWorldPos(const Vector3<GLfloat> &v_WorldPos)
{
    WorldPos.x = v_WorldPos.x;
    WorldPos.y = v_WorldPos.y;
    WorldPos.z = v_WorldPos.z;
}


void objectTransform::SetRotate(const GLfloat &RotateX, const GLfloat &RotateY, const GLfloat &RotateZ)
{
    Rotate.x = RotateX;
    Rotate.y = RotateY;
    Rotate.z = RotateZ;
}

void objectTransform::SetRotate(const Vector3<GLfloat> &v_Rotate)
{
    Rotate.x = v_Rotate.x;
    Rotate.y = v_Rotate.y;
    Rotate.z = v_Rotate.z;
}


void objectTransform::SetTransform(const objectTransform &trans)
{
    SetScale(trans.GetScale());
    SetRotate(trans.GetRotate());
    SetWorldPos(trans.GetWorldPos());
}

void objectTransform::Move(const GLfloat &X, const GLfloat &Y, const GLfloat &Z)
{
    SetWorldPos(WorldPos.x + X, WorldPos.y + Y, WorldPos.z + Z);
}
