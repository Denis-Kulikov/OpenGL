#include <entities/actor.hpp>
#include <entities/templates/decor/wave.hpp>

std::map<std::string, Sprite> Wave::Sprites;
Bone Wave::skelet;
size_t Wave::skeletSize;

Wave::Wave()
    : Actor(std::string("decor/wave"), GetSkeletSize())
{
    name = "wave";
}

Wave::~Wave()
{
    // delete[] components;
    // delete[] animations;
}

void Wave::Initialize()
{
    std::string path("decor/wave");
    name = "wave";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Wave>(path, name, _animations);

    std::cout << "wave skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

size_t Wave::GetSkeletSize() {
    return Wave::skeletSize;
}

Bone *Wave::GetSkelet() {
    return &Wave::skelet;
}

std::map<std::string, Sprite> *Wave::GetSprites() {
    return &Wave::Sprites;
}

std::string *Wave::GetName() {
    return &Wave::name;
}
