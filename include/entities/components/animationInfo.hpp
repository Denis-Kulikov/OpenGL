#pragma once

#include "animation.hpp"
#include "_motion.hpp"

class AnimationInfo
{
public:
    AnimationInfo() {};
    ~AnimationInfo() {
        delete globalFlip;
        delete components;
        delete animations;
        delete animations[0];   
    };

    void Initialize(size_t SkeletSize)
    {
        globalFlip = new GLfloat[SkeletSize];
        transformations = new float[SkeletSize * 5];
        components = new Component[SkeletSize];
        animations = new Animation*[SkeletSize];
        for (int i = 0; i < SkeletSize; i++) {
            animations[i] = nullptr;
            for (int j = 0; j < 5; j++)
                transformations[5 * i + j] = 0.0;
        }
        animations[0] = new Animation();
        globalFlip[0] = 0;
    };



    GLfloat *globalFlip = nullptr;
    float *transformations = nullptr;
    Component *components = nullptr;
    Animation **animations = nullptr;
    float AnimationTimeStart = 0.0;
    std::string animation;
    // motion *_motion;
};

