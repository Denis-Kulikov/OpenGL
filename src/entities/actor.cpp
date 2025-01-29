#include <entities/actor.hpp>

Actor::Actor(const std::string &path)
{
}

Actor::~Actor() {};


void Actor::Render(void *RenderData) const {
    if (mesh != nullptr) {
        static_cast<Actor_rdata*>(RenderData)->mesh->Render(static_cast<Actor_rdata*>(RenderData)->BonesTransforms);
    }
    
    if (rootComponent != nullptr) {
        rootComponent->UpdateMatrixTree();
        rootComponent->RenderTree();
    }
}


void Actor::updateAnimation() {}


bool Actor::loadActor(const std::string &path)
{
    return true;
}

void Actor::Spawn(const Transform *startTransform)
{
    if (rootComponent == nullptr) return;
    rootComponent->Spawn(startTransform);
}


objectTransform *Actor::GetTransform()
{
    return &transform;
}

glm::vec3 Actor::GetDirection() const
{
    return direction;
}

void Actor::SetDirection(const glm::vec3 &_direction)
{
    direction = _direction;
}
