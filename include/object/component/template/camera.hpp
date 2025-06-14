#pragma once
#include <render/camera.hpp>
#include "../component.hpp"

class ComponentCamera : public Component {
public:
    ComponentCamera(Transform *transform)
        : Component(transform) {}

    Camera camera;
};
