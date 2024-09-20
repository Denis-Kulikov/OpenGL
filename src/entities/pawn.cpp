#include <entities/pawn.hpp>
#include <game/gameManager.hpp>

Pawn::Pawn(const std::string &path, const size_t SkeletSize)
    : Actor(path, SkeletSize)
{}

void Pawn::Teleport(const glm::vec3 newPosition) {
    this->transform.WorldPos = newPosition;
}

void Pawn::Move(const glm::vec3 offset) {
    this->transform.Move(offset);
}

void Pawn::Move(const GLfloat distance, const glm::vec3 direction) {
    this->transform.Move(distance, direction);
}

void Pawn::MoveForward(const GLfloat distance) {
    const GLfloat y = this->direction.y;

    GLfloat forwardX = cos(ToRadian(GetYaw())) * this->direction.z;
    GLfloat forwardZ = sin(ToRadian(GetYaw())) * this->direction.z;

    GLfloat rightX = cos(ToRadian(GetYaw() + 90.0f)) * this->direction.x;
    GLfloat rightZ = sin(ToRadian(GetYaw() + 90.0f)) * this->direction.x;

    GLfloat finalX = forwardX + rightX;
    GLfloat finalZ = forwardZ + rightZ;

    glm::vec3 finalDirection(finalX, y, finalZ);
    // if (finalDirection.Length() != 0) {
    //     finalDirection.Normalize();
    // }

    this->transform.Move(distance, finalDirection);
}

void Pawn::MoveForward() {
    MoveForward(this->speed * GameManager::Time.GetDeltaTime());
}

void Pawn::Rotate(const glm::vec3 _rotate) {
    this->transform.Rotate = _rotate;
}

void Pawn::AddRotate(const glm::vec3 _rotate) {
    this->transform.Rotate += _rotate;
}

void Pawn::SetScale(const glm::vec3 _scale) {
    this->transform.Scale = _scale;
}

void Pawn::MultiplyScale(const glm::vec3 _scale) {
    this->transform.Scale *= _scale;
}

GLfloat Pawn::GetSpeed() const
{
    return speed;
}

GLfloat Pawn::GetYaw() const
{
    return yaw;
}
GLfloat Pawn::GetPitch() const
{
    return pitch;
}

void Pawn::SetYaw(GLfloat new_yaw)
{
    yaw = new_yaw;
}
void Pawn::SetPitch(GLfloat new_pitch)
{
    pitch = new_pitch;
}
