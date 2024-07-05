#pragma once
#include "sprite.hpp"
#include "sphere.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "cube_bone.hpp"
#include "../entities/components/component.hpp"
#include "../entities/actor.hpp"

class Scene 
{
public:
    Scene() {};
    ~Scene() {
        for (auto& it : actors) delete it;
    };
    void pushActor(Actor* actor) { actors.push_back(actor); };
    std::vector<Actor*> actors;
};
