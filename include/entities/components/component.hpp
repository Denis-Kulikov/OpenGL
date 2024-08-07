#pragma once
#include "../../object/sprite.hpp"

class Component
{
public:
    Component(objectTransform &_transform, Sprite *_sprite) : transform(_transform), sprite(_sprite) {};
    Component(objectTransform _transform, Sprite *_sprite) : transform(_transform), sprite(_sprite) {};
    Component() : Component(objectTransform(), nullptr) {}

    objectTransform transform;
    Sprite *sprite = nullptr;
};