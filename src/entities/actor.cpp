#include <entities/actor.hpp>
#include <managers/time_manager.hpp>

Actor::Actor() {
    speed = 5.0;
}
Actor::~Actor() {};

void Actor::Render() const {
    if (rootComponent != nullptr) {
        rootComponent->UpdateMatrixTree();
        rootComponent->RenderTree();
    }
}

void Actor::updateAnimation() {}

void Actor::Spawn(Transform& startTransform)
{
    if (rootComponent == nullptr) return;
    rootComponent->Spawn(startTransform);
}


glm::vec3 Actor::GetDirection() const
{
    return direction;
}

void Actor::SetDirection(const glm::vec3 &_direction)
{
    direction = _direction;
}

void Actor::Teleport(const glm::vec3 newPosition) {
    if (rootComponent != nullptr)
        rootComponent->SetPosition(newPosition);
}

void Actor::Move(const glm::vec3 offset) {
    if (rootComponent != nullptr)
        rootComponent->Move(offset);
}

void Actor::Move(const glm::vec3 direction, const float distance) {
    if (rootComponent != nullptr) 
        rootComponent->Move(direction, distance);
}

void Actor::MoveForward(const float distance) {
    Move(direction, distance);
}

void Actor::MoveForward() {
    MoveForward(speed * TimeManager::GetDeltaTime());
}

void Actor::MoveTowards(const glm::vec3 target, const float distance) {
    if (rootComponent != nullptr) {
        glm::vec3 direction = target - rootComponent->GetPosition();
        Move(direction, distance);
    }
}

void Actor::Rotate(const glm::vec3 rotate) {
    if (rootComponent != nullptr)
        rootComponent->SetRotation(rotate);
}

void Actor::AddRotate(const glm::vec3 rotate) {
    if (rootComponent != nullptr) {
        glm::quat currentRotation = rootComponent->GetRotation();
        glm::quat deltaRotation = glm::quat(glm::radians(rotate));
        glm::quat newRotation = deltaRotation * currentRotation;
        rootComponent->SetRotation(glm::normalize(newRotation));
    }
}

void Actor::SetScale(const glm::vec3 scale) {
    if (rootComponent != nullptr)
        rootComponent->SetScale(scale);
}

void Actor::MultiplyScale(const glm::vec3 scale) {
    if (rootComponent != nullptr)
        rootComponent->SetScale(rootComponent->GetScale() * scale);
}

float Actor::GetSpeed() const {
    return speed;
}
