#pragma once
#include <entities/actor.hpp>
#include <object/component/template/camera.hpp>

class Pipeline
{
public:
    Pipeline(const Camera &_camera);
    Pipeline();
    
    void SetCamera(const Camera &_camera);
    void drawSkybox(Actor &skybox);

    // Perspective - true, Orthographic - false
    // Рендер использует Left-Handed (LH) координатная система
    // glm::perspective и glm::lookAt не подходят. Используйте LookAtLH и PerspectiveLH
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
