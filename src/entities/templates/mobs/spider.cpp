#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>

std::map<std::string, Sprite> Spider::Sprites;
size_t Spider::skeletSize = 0;
Bone Spider::skelet;

Spider::Spider() : Character(std::string("mobs/spider"), GetSkeletSize())
{
    name = "Spider";
}

Spider::~Spider()
{
    delete[] components;
    delete[] animations;
}

void Spider::Initialize()
{
    std::string path("mobs/spider");
    name = "spider";
    std::vector<std::string> _animations = {"stand"};
    Actor::Initialize<Spider>(path, name, _animations);

    std::cout << "Spider skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}

size_t Spider::GetSkeletSize() {
    return Spider::skeletSize;
}

Bone *Spider::GetSkelet() {
    return &Spider::skelet;
}

std::map<std::string, Sprite> *Spider::GetSprites() {
    return &Spider::Sprites;
}

std::string *Spider::GetName() {
    return &Spider::name;
}
