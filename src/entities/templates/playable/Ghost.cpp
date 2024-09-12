#include <entities/templates/playable/Ghost.hpp>

Ghost::Ghost()
    : Character(std::string("player/Ghost"), GetSkeletSize())
{
    name = "Ghost";
    motionPtr = &motion;
}

Ghost::~Ghost()
{}

void Ghost::Initialize()
{
    std::string path("player/Ghost");
    name = "Ghost";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Ghost>(path, name, _animations);

    std::cout << "Ghost skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Ghost::SetMotion()
{
    motion = Motion();

    Motion::FunType stand = [&]() {};

    std::pair<float, Motion::FunType> _stand = {1.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Ghost::GetSkeletSize() const {
    return Ghost::skeletSize;
}

Bone *Ghost::GetSkelet() const {
    return &Ghost::skelet;
}

std::map<std::string, Sprite> *Ghost::GetSprites() const {
    return &Ghost::Sprites;
}

std::string *Ghost::GetName() const {
    return &Ghost::name;
}
