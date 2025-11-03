#pragma once
#include <scene/camera.hpp>
#include "../component.hpp"
#include <memory>

class ComponentCamera : public Component {
public:
    ComponentCamera(TransformableMatrix *transform)
        : Component(transform)
    {
        castsShadow = false;
    }

    ComponentCamera()
    {
        castsShadow = false;
    }

    // Рендер использует Left-Handed (LH) координатная система
    // glm::perspective и glm::lookAt не подходят. Используйте LookAtLH и PerspectiveLH.
    void UpdatePV();
    // Perspective - true, Orthographic - false.
    void UpdateProj(bool perspective = true);
    void UpdateView();
    void UpdatePerspective();
    void UpdateOrthographic();

    Camera camera;
    glm::mat4 Proj;
    glm::mat4 View;
    glm::mat4 PV;
};
