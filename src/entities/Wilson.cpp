#include <entities/templates/playable/Wilson.hpp>

template<>
std::map<std::string, Sprite> Actor<Wilson>::Sprites;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    std::string path("player/Wilson");
    skelet.createSkelet(path, "skelet");
    // loadAnimation(path, "stand", &skelet, &Sprites);
    sprites = &Sprites;
}