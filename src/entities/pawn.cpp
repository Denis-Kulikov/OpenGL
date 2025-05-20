#include <managers/time_manager.hpp>
#include <entities/pawn.hpp>

Pawn::Pawn(const std::string &path)
    : Actor(path)
{
    speed = 5.0;
}

void Pawn::Teleport(const glm::vec3 newPosition) {
    if (rootComponent != nullptr)
        rootComponent->SetPosition(newPosition);
}

void Pawn::Move(const glm::vec3 offset) {
    if (rootComponent != nullptr)
        rootComponent->Move(offset);
}

void Pawn::Move(const glm::vec3 direction, const float distance) {
    if (rootComponent != nullptr) 
        rootComponent->Move(direction, distance);
}

void Pawn::MoveForward(const float distance) {
    Move(direction, distance);
}

void Pawn::MoveForward() {
    MoveForward(speed * TimeManager::GetDeltaTime());
}

void Pawn::MoveTowards(const glm::vec3 target, const float distance) {
    if (rootComponent != nullptr) {
        glm::vec3 direction = target - rootComponent->GetPosition();
        Move(direction, distance);
    }
}

void Pawn::Rotate(const glm::vec3 rotate) {
    if (rootComponent != nullptr)
        rootComponent->SetRotation(rotate);
}

void Pawn::AddRotate(const glm::vec3 rotate) {
    if (rootComponent != nullptr) {
        glm::quat currentRotation = rootComponent->GetRotation();
        glm::quat deltaRotation = glm::quat(glm::radians(rotate));
        glm::quat newRotation = deltaRotation * currentRotation;
        rootComponent->SetRotation(glm::normalize(newRotation));
    }
}

void Pawn::SetScale(const glm::vec3 scale) {
    if (rootComponent != nullptr)
        rootComponent->SetScale(scale);
}

void Pawn::MultiplyScale(const glm::vec3 scale) {
    if (rootComponent != nullptr)
        rootComponent->SetScale(rootComponent->GetScale() * scale);
}

float Pawn::GetSpeed() const {
    return speed;
}
