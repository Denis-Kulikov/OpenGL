#pragma once

#include "../../character.hpp"

class Wilson : public Character
{
public:
    Wilson() : Character(std::string("player/Wilson"))
    {
        std::string path("player/Wilson");
        Wilson::loadSprites(path);
        // character = new Character(path);
        skelet.createSkelet(path, "skelet");
        Wilson::loadAnimation(path, "stand");
    }
    
protected:
    static std::map<std::string, Sprite> Sprites;

};
