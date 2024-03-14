#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;
size_t Wilson::skeletSize;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    components = new Component[skeletSize];
    for (int i = 0; i < skeletSize; i++)
        components[i] = Component();

}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    loadSkelet(path);
    loadSprites(path);
    loadAnimation(path, "stand");

    std::cout << "Wilson skelet size: " << skeletSize << std::endl;
    skelet.printBones(0);
}
