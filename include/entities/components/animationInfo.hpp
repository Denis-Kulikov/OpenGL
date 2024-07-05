#pragma once

#include "animation.hpp"
#include "motion.hpp"

class AnimationInfo
{
public:
    AnimationInfo(std::size_t SkeletSize)
        : globalFlip(SkeletSize), transforms(SkeletSize), components(SkeletSize), animations(SkeletSize)
    {}

    std::vector<GLfloat> globalFlip;
    std::vector<Motion::bone_attribute> transforms;
    std::vector<Component> components;
    std::vector<Animation> animations;
    float AnimationTimeStart = 0.0;
    std::string animation;
};
