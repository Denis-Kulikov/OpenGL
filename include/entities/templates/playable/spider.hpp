#pragma once

#include "../../character.hpp"

class Spider : public Character<Spider>
{
public:
    Spider() : Character(std::string("mobs/spider"))
    {
    }

    static void Initialize()
    {
        std::string path("mobs/spider");
        loadSprites(path);
        skelet.createSkelet(path, "skelet");
        loadAnimation(path, "stand");
    }
    
    static std::map<std::string, Sprite> Sprites;
    static Bone skelet;
};
