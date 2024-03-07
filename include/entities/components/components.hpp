#pragma once
#include "../../object/sprite.hpp"

class Components
{
public:
    Components() {};

    void SetChildAnimation(const objectTransform &_transform);

    std::string name;
    objectTransform trans;
    // objectTransform anchorPoint;
    Sprite *sprite;
    // float time = 0;

    // sprite *getSprite();             // получение спрайта от анимации
    // objectTransform *getTransform(); // получение данных trans от кости
};
