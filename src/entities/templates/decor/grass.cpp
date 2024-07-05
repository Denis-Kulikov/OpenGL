#include <entities/actor.hpp>
#include <entities/templates/decor/grass.hpp>

std::map<std::string, Sprite> Grass::Sprites;
Bone Grass::skelet;
size_t Grass::skeletSize;

Grass::Grass()
    : Actor(std::string("decor/wave"), GetSkeletSize())
{
    name = "grass";
    motionPtr = &motion;
}

Grass::~Grass()
{}

void Grass::Initialize()
{
    std::string path("decor/grass");
    name = "grass";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Grass>(path, name, _animations);

    std::cout << "grass skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

void Grass::SetMotion()
{
    motion = Motion();
    motion.PushSkelet(&skelet);

    Motion::FunType stand = [&]() {};

    std::pair<float, Motion::FunType> _stand = {1.0, stand};
    motion.PushMotion("stand", _stand);
}

size_t Grass::GetSkeletSize() const {
    return Grass::skeletSize;
}

Bone *Grass::GetSkelet() const {
    return &Grass::skelet;
}

std::map<std::string, Sprite> *Grass::GetSprites() const {
    return &Grass::Sprites;
}

std::string *Grass::GetName() const {
    return &Grass::name;
}
