#pragma once

#include "../../character.hpp"

class Wilson : public Character<Wilson>
{
public:
    Wilson();
    ~Wilson();
    
    static void Initialize();

    static size_t skeletSize;
    static Bone skelet;
    static std::map<std::string, Sprite> Sprites;
};
