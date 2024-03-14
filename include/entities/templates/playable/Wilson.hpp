#pragma once

#include "../../character.hpp"

class Wilson : public Character<Wilson>
{
public:
    Wilson();
    
    static void Initialize();

    static std::map<std::string, Sprite> Sprites;
    static Bone skelet;
};
