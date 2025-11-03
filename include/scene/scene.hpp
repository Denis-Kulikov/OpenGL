#pragma once
#include <entities/actor.hpp>
#include <managers/render/render.hpp>
#include "light/scene_lighting.hpp"
#include "shadow/scene_shadow.hpp"
#include <object/component/template/camera.hpp>
// #include <game/bulletManager.hpp>

class Scene 
{
public:
    Scene();
    ~Scene();
    void SetCamera(ComponentCamera *camera);
    void UpdateCamera();
    void pushObject(Actor* actor);
    void Update(float deltaTime);
    void Render();
    void DrawSkybox();
    void Callback(float deltaTime);
 
    std::vector<Actor*> actors;
    SceneLighting lighting;
    SceneShadow shadow;
    Actor* skybox = nullptr;

    ComponentCamera* curCamera = nullptr;
};
