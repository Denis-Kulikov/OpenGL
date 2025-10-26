#pragma once
#include <scene/camera.hpp>
#include "../component.hpp"

class ComponentCamera : public Component {
public:
    ComponentCamera(TransformableMatrix *transform)
        : Component(transform)
    {
        castsShadow = false;
    }

    Camera camera;
};
