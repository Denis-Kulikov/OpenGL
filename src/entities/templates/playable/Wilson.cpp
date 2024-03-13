#include <entities/templates/playable/Wilson.hpp>


template <typename Derived>
Pawn<Derived>::Pawn(const std::string &path) : Actor<Derived>(path) 
{
    cameraPos = Vector3<GLfloat>(0.0f, 6.0f, 15);
}

template <typename Derived>
Character<Derived>::Character(const std::string &path) : Pawn<Derived>(path) {}


std::map<std::string, Sprite> Wilson::Sprites;

Wilson::Wilson()
    : Character<Wilson>(std::string("player/Wilson"))
{
    std::string path("player/Wilson");
    skelet.createSkelet(path, "skelet");
    // loadAnimation(path, "stand", &skelet, &Sprites);
    sprites = &Sprites;
}