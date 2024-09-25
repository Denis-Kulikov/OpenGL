#include <entities/actor.hpp>

Actor::Actor(const std::string &path)
{
}

Actor::~Actor() {};


void Actor::Render(void *RenderData) const {
    static_cast<Actor_rdata*>(RenderData)->mesh->Render(static_cast<Actor_rdata*>(RenderData)->BonesTransforms);
    delete static_cast<Actor_rdata*>(RenderData)->BonesTransforms;
}


void Actor::updateAnimation()
{

}



bool Actor::loadActor(const std::string &path)
{
    return true;
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
