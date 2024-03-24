#pragma once
#include "component.hpp"
#include "motion.hpp"

class Animation
{
public:
    Animation(Sprite *_sprite, const Vector3<GLfloat> &_spriteScale, const objectTransform &_transform) 
        : sprite(_sprite), spriteScale(_spriteScale), transform(_transform) {};
    Animation() {};

    void SetChildAnimation(const objectTransform &_transform);

    Sprite *sprite = nullptr;
    // TimeType end;
    Vector3<GLfloat> spriteScale;
    Vector3<GLfloat> anchorPoint;
    objectTransform transform;
    Motion motion;
};
