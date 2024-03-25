#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>

std::map<std::string, Sprite> Spider::Sprites;
size_t Spider::skeletSize = 0;
Bone Spider::skelet;

Spider::Spider() : Character(std::string("mobs/spider"))
{
    name = "Spider";
}

Spider::~Spider()
{}

void Spider::Initialize()
{
    std::string path("mobs/spider");
    name = "spider";
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");

    std::cout << "Spider skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}
    