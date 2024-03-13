#pragma once

#include "../../character.hpp"

class Wilson : public Character<Wilson>
{
public:
    Wilson();
    // Wilson() : Character<Wilson>(std::string("player/Wilson"))
    // {
    //     std::string path("player/Wilson");
    //     skelet.createSkelet(path, "skelet");
    //     loadAnimation(path, "stand", &skelet, &Sprites);
    //     sprites = &Sprites;
    // }
    
    static std::map<std::string, Sprite> Sprites;
};
