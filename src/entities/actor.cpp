#include <entities/actor.hpp>

Actor::Actor(const std::string &path) {}
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
