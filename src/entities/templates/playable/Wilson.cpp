#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character(std::string("player/Wilson"), GetSkeletSize())
{
    name = "Wilson";
}

Wilson::~Wilson()
{
    delete[] components;
    delete[] animations;

    std::cout << "Wilson::~Wilson()" << std::endl;
}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    name = "Wilson";
    std::vector<std::string> _animations = {"stand", "stand_2"};
    Actor::Initialize<Wilson>(path, name, _animations);

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

size_t Wilson::GetSkeletSize() {
    return Wilson::skeletSize;
}

Bone *Wilson::GetSkelet() {
    return &Wilson::skelet;
}

std::map<std::string, Sprite> *Wilson::GetSprites() {
    return &Wilson::Sprites;
}

std::string *Wilson::GetName() {
    return &Wilson::name;
}
