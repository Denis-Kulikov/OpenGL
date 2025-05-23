#pragma once
#include "../entities/actor.hpp"
// #include <game/bulletManager.hpp>

class Scene 
{
public:
    Scene() {};
    ~Scene() {
        for (auto& it : actors) delete it;
    };
    void pushObject(Actor* actor) { actors.push_back(actor); };
    
    std::vector<Actor*> actors;
    // BulletManager btManager;
    Actor* skybox;
};
