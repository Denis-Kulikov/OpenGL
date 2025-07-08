#pragma once
#include "../entities/actor.hpp"
#include <object/component/template/camera.hpp>

class Pipeline
{
public:
    Pipeline(const Camera &_camera);
    Pipeline();
    
    void SetCamera(const Camera &_camera);
    void drawSkybox(Actor &skybox);

    glm::mat4 GetModel(const Transform& transform) const;
    // Perspective - true, Orthographic - false
    void UpdatePV();
    void UpdateProj(bool perspective);
    void UpdateView();
    void UpdatePerspective();
    void UpdateOrthographic();

    ComponentCamera* camera = nullptr;

    glm::mat4 ProjTrans;
    glm::mat4 View;
    glm::mat4 PV;
};
