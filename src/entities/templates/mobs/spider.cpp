#include <entities/actor.hpp>
#include <entities/templates/mobs/spider.hpp>

std::map<std::string, Sprite> Spider::Sprites;
size_t Spider::skeletSize = 0;
Bone Spider::skelet;

Spider::Spider() : Character(std::string("mobs/spider"))
{
    components = new Component[skeletSize];
    animations = new Animation*[skeletSize];
    for (int i = 0; i < skeletSize; i++) {
        components[i] = Component();
        animations[i] = nullptr;
    }
    animations[0] = new Animation();
}


Spider::~Spider()
{
}


void Spider::Initialize()
{
    std::string path("mobs/spider");
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");

    std::cout << "Spider skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}
    