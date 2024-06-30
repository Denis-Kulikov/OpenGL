#pragma once

#include "animation.hpp"
#include "motion.hpp"

class AnimationInfo
{
public:
    AnimationInfo() {}
    ~AnimationInfo() {}

    void Initialize(std::size_t SkeletSize)
    {
        skeletSize = SkeletSize;

        globalFlip.reserve(SkeletSize);
        transforms.reserve(SkeletSize);
        components.reserve(SkeletSize);
        animations.reserve(SkeletSize);
        animations.reserve(SkeletSize);
        globalFlip.reserve(SkeletSize);

        for (size_t i = 0; i < skeletSize; ++i) {
            globalFlip.emplace_back(GLfloat());
            transforms.emplace_back(Motion::bone_attribute());
            components.emplace_back(Component());
            animations.emplace_back(Animation());
            animations.emplace_back(Animation());
            globalFlip.emplace_back(0);
        }
    }

    std::vector<GLfloat> globalFlip;
    std::vector<Motion::bone_attribute> transforms;
    std::vector<Component> components;
    std::vector<Animation> animations;
    float AnimationTimeStart = 0.0;
    std::string animation;
    std::size_t skeletSize = 0;
};
