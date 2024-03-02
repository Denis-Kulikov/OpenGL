#pragma once
#include "../object/sprite.hpp"

class transition
{
public:
    transition();

    std::string name;
    
    component *parent = nullptr;
    std::vector<component*> children;
    fload speed;

    std::vector<sprite*> sprites;


    int zIndex;
};
