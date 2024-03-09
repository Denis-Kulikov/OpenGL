#pragma once
#include "../../object/sprite.hpp"
#include "component.hpp"

class Animation
{
public:
    Animation() {};

    void SetChildAnimation(const objectTransform &_transform);

    std::string name;
    objectTransform trans;
    Component component;
    Vector3<GLfloat> spriteScale;
    Sprite *sprite = nullptr;
    // objectTransform anchorPoint;
};
