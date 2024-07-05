#pragma once
#include "component.hpp"

class Animation
{
public:
    Animation(Sprite *_sprite, const Vector3<GLfloat> &_spriteScale, const objectTransform &_transform) 
        : sprite(_sprite), spriteScale(_spriteScale), transform(_transform) {};
    Animation() {};

    static void PushDuration(const std::string &actorName, const std::string &animationName, const float _duration)
    {
        duration[actorName][animationName] = _duration;
    }

    static float GetDuration(const std::string &actorName, const std::string &animationName)
    {
        auto actor = duration.find(actorName);
        if (actor != duration.end()) {
            auto it = actor->second.find(animationName);
            if (it != actor->second.end()) {
                return it->second;
            }
        } 
        
        return 0.0;
    }

    Sprite *sprite = nullptr;
    Vector3<GLfloat> spriteScale;
    Vector3<GLfloat> anchorPoint;
    objectTransform transform;
    static inline std::map<std::string, std::map<std::string, float>> duration;
};
