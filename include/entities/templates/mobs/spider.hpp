#pragma once

#include "../../character.hpp"

class Spider : public Character<Spider>
{
public:
    Spider();
    ~Spider();

    static void Initialize();
    
    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
