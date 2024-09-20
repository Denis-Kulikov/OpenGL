#include <object/objectTransform.hpp>

glm::vec3 objectTransform::GetScale() const
{
    return Scale;
}

glm::vec3 objectTransform::GetWorldPos() const
{
    return WorldPos;
}

glm::vec3 objectTransform::GetRotate() const
{
    return Rotate;
}


void objectTransform::SetScale(const GLfloat &ScaleX, const GLfloat &ScaleY, const GLfloat &ScaleZ)
{
    Scale.x = ScaleX;
    Scale.y = ScaleY;
    Scale.z = ScaleZ;
}

void objectTransform::SetScale(const glm::vec3 &v_Scale)
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

void objectTransform::SetWorldPos(const glm::vec3 &v_WorldPos)
{
    WorldPos.x = v_WorldPos.x;
    WorldPos.y = v_WorldPos.y;
    WorldPos.z = v_WorldPos.z;
}

float objectTransform::GetRotateTowards(const glm::vec3 &target_pos)
{
    glm::vec3 direction;
    direction.x = WorldPos.x - target_pos.x;
    direction.y = 0.0f;
    direction.z = WorldPos.z - target_pos.z;

    direction = glm::normalize(direction);
    // glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
    float angle = glm::degrees(atan2(direction.z, direction.x));

    return angle;
}

void objectTransform::SetRotate(const GLfloat &RotateX, const GLfloat &RotateY, const GLfloat &RotateZ)
{
    Rotate.x = RotateX;
    Rotate.y = RotateY;
    Rotate.z = RotateZ;
}

void objectTransform::SetRotate(const glm::vec3 &v_Rotate)
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

void objectTransform::Move(const glm::vec3 offset) {
    WorldPos += glm::vec3(offset.x, offset.y, offset.z);
}

void objectTransform::Move(const GLfloat distance, const glm::vec3 direction) {
    glm::vec3 offset = direction;
    if (glm::length(offset) != 0) {
        glm::normalize(offset);
    }
    offset *= distance;
    Move(offset);
}

void objectTransform::MoveForward(const GLfloat distance, const enum AXES axis) {
    glm::vec3 forward = GetForwardVector(axis);
    Move(distance, forward);
}

glm::vec3 objectTransform::GetForwardVector(const enum AXES axis) const
{
    GLfloat radianAngle = ToRadian(Rotate[axis]);

    switch (axis) {
    case X:
        return glm::vec3(0.0, cos(radianAngle), sin(radianAngle));
    case Y:
        return glm::vec3(cos(radianAngle), 0.0f, sin(radianAngle));
    case Z:
        return glm::vec3(cos(radianAngle), sin(radianAngle), 0.0);
    default:
        return glm::vec3(0.0, 0.0, 0.0);
    }
}

void objectTransform::AddRotate(const glm::vec3 _rotate) {
    Rotate += _rotate;
}


void objectTransform::MultiplyScale(const glm::vec3 _scale) {
    Scale *= _scale;
}

std::ostream& operator<<(std::ostream& os, const objectTransform& transform)
{
    os << "World Position: (" << transform.WorldPos.x << ", " << transform.WorldPos.y << ", " << transform.WorldPos.z << ")\n"
       << "Rotation: (" << transform.Rotate.x << ", " << transform.Rotate.y << ", " << transform.Rotate.z << ")\n"
       << "Scale: (" << transform.Scale.x << ", " << transform.Scale.y << ", " << transform.Scale.z << ")\n";

    return os;
}
