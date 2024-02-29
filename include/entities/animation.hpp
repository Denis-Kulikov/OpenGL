#pragma once
#include "../object/sprite.hpp"

class animation
{
public:
    animation();

    std::string name;
    objectTransform trans;
    objectTransform anchorPoint;

    component *parent = nullptr;
    std::vector<component*> children;

    int zIndex;
};
