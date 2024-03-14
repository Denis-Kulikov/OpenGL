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
    
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
