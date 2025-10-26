#pragma once
#include <entities/actor.hpp>
#include <managers/render/render.hpp>
#include "light/scene_lighting.hpp"
#include "shadow/scene_shadow.hpp"
// #include <game/bulletManager.hpp>

class Scene 
{
public:
    Scene();
    ~Scene();
    void pushObject(Actor* actor);
    
    std::vector<Actor*> actors;
    SceneLighting lighting;
    SceneShadow shadow;
    Actor* skybox;
};
