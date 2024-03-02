#pragma once
#include "../object/sprite.hpp"
#include "../object/sprite.hpp"

class animation
{
public:
    animation();

    std::string name;
    
    std::vector<transition*> transitions;
    std::vector<sprite*> sprites;
    int zIndex;
};
