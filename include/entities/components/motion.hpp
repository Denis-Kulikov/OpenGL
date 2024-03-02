#pragma once
#include "../object/sprite.hpp"

class motion
{
public:
    motion();

    std::string name;
    
    component *parent = nullptr;
    std::vector<component*> children;
    fload speed;



    int zIndex;
};
