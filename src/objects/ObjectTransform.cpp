#include <object/objectTransform.hpp>

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

void objectTransform::Move(const Vector3<GLfloat> offset) {
    WorldPos += Vector3<GLfloat>(offset.x, offset.y, offset.z);
}

void objectTransform::Move(const GLfloat distance, const Vector3<GLfloat> direction) {
    Vector3<GLfloat> offset = direction;
    if (offset.Length() != 0) {
        offset.Normalize();
    }
    offset *= distance;
    Move(offset);
}

void objectTransform::MoveForward(const GLfloat distance, const enum AXES axis) {
    Vector3<GLfloat> forward = GetForwardVector(axis);
    Move(distance, forward);
}

Vector3<GLfloat> objectTransform::GetForwardVector(const enum AXES axis) const
{
    GLfloat radianAngle = ToRadian(Rotate[axis]);

    switch (axis) {
    case X:
        return Vector3<GLfloat>(0.0, cos(radianAngle), sin(radianAngle));
    case Y:
        return Vector3<GLfloat>(cos(radianAngle), 0.0f, sin(radianAngle));
    case Z:
        return Vector3<GLfloat>(cos(radianAngle), sin(radianAngle), 0.0);
    default:
        return Vector3<GLfloat>(0.0, 0.0, 0.0);
    }
}

void objectTransform::AddRotate(const Vector3<GLfloat> _rotate) {
    Rotate += _rotate;
}


void objectTransform::MultiplyScale(const Vector3<GLfloat> _scale) {
    Scale *= _scale;
}


std::ostream& operator<<(std::ostream& os, const objectTransform& transform)
{
    os << "World Position: (" << transform.WorldPos.x << ", " << transform.WorldPos.y << ", " << transform.WorldPos.z << ")\n"
       << "Rotation: (" << transform.Rotate.x << ", " << transform.Rotate.y << ", " << transform.Rotate.z << ")\n"
       << "Scale: (" << transform.Scale.x << ", " << transform.Scale.y << ", " << transform.Scale.z << ")\n";

    return os;
}
