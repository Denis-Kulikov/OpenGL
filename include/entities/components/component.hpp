#pragma once
#include "../../object/sprite.hpp"

class Component
{
public:
    Component(objectTransform &_transform, Sprite *_sprite) : transform(_transform), sprite(_sprite) {};
    Component()
    {
        transform = objectTransform();
        sprite = nullptr;
    }

    objectTransform transform;
    Sprite *sprite = nullptr;
};