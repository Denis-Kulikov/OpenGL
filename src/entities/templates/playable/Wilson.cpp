#include <entities/actor.hpp>
#include <entities/templates/playable/Wilson.hpp>

std::map<std::string, Sprite> Wilson::Sprites;
Bone Wilson::skelet;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{}

void Wilson::Initialize()
{
    std::string path("player/Wilson");
    skelet.createSkelet(path, "skelet");
    loadSprites(path);
    loadAnimation(path, "stand");
}
