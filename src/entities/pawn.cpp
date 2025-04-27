#include <entities/pawn.hpp>
#include <game/gameManager.hpp>

Pawn::Pawn(const std::string &path)
    : Actor(path)
{
    speed = 5.0;
}

void Pawn::Teleport(const glm::vec3 newPosition) {
    transform.WorldPos = newPosition;
}

void Pawn::Move(const glm::vec3 offset) {
    transform.Move(offset);
}

void Pawn::Move(const GLfloat distance, const glm::vec3 direction) {
    transform.Move(distance, direction);
}

void Pawn::MoveForward(const GLfloat distance) {
    transform.Move(distance, direction);
}

void Pawn::MoveForward() {
    MoveForward(speed * GameManager::Time.GetDeltaTime());
}

void Pawn::MoveTowards(const glm::vec3 target, const GLfloat speed) {
    glm::vec3 direction = target - transform.WorldPos;
    Move(speed * GameManager::Time.GetDeltaTime(), direction);
}

void Pawn::Rotate(const glm::vec3 rotate) {
    transform.Rotate = rotate;
}

void Pawn::AddRotate(const glm::vec3 rotate) {
    transform.Rotate += rotate;
}

void Pawn::SetScale(const glm::vec3 scale) {
    transform.Scale = scale;
}

void Pawn::MultiplyScale(const glm::vec3 scale) {
    transform.Scale *= scale;
}

GLfloat Pawn::GetSpeed() const {
    return speed;
}
