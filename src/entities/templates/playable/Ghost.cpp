#include <entities/templates/playable/Ghost.hpp>
#include <object/component/template/point.hpp>

Ghost::Ghost()
    : Character(std::string("player/Ghost"))
{
        Transform *transform = new Transform();
        ComponentPoint *point = CreateComponent<ComponentPoint>(transform);
        rootComponent = point;
}

Ghost::~Ghost()
{}

void Ghost::Initialize()
{
    std::string path("player/Ghost");
    name = "Ghost";
}

std::string Ghost::GetName() const {
    return name;
}
