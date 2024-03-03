#pragma once
#include "../object/sprite.hpp"
#include "../object/sprite.hpp"

class Animation
{
public:
    Animation();

    std::string name;
    
    std::pair<std::string, std::string> *sprites;
};
