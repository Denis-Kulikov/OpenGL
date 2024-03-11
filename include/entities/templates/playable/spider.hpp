#pragma once

#include "../../character.hpp"

class Spider : public Character
{
public:
    Spider() : Character(std::string("mobs/spider"))
    {
        std::string path("mobs/spider");
        Spider::loadSprites(path);
        // character = new Character(path);
        skelet.createSkelet(path, "skelet");
        Spider::loadAnimation(path, "stand");
    }
    
protected:
    static std::map<std::string, Sprite> Sprites;

};
