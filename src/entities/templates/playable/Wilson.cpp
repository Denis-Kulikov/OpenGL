#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    components = new Component[skeletSize];
    animations = new Animation*[skeletSize];
    for (int i = 0; i < skeletSize; i++) {
        components[i] = Component();
        animations[i] = nullptr;
    }
    animations[0] = new Animation();
}

Wilson::~Wilson()
{
    std::cout << "Wilson::~Wilson()" << std::endl;
}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");
    loadAnimation(path, "stand_2");

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
    std::cout << std::endl;
}
