#pragma once
#include "../entities/actor.hpp"
#include <object/component/template/camera.hpp>

class Render
{
public:
    Render(const Camera &_camera);
    Render();
    
    void SetCamera(const Camera &_camera);
    void drawSkybox(Actor &skybox);

    glm::mat4 GetModel(const Transform& transform) const;
    glm::mat4 GetPV();
    // Perspective - true, Orthographic - false
    void UpdatePV(bool perspective);
    void UpdatePV_Perspective();
    void UpdatePV_Orthographic();

    ComponentCamera* camera = nullptr;

    glm::mat4 ProjTrans;
    glm::mat4 View;
    glm::mat4 PV;
};
