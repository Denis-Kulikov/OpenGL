#pragma once
#include <btBulletDynamicsCommon.h>
#include <render/camera.hpp>
#include "../component.hpp"

class ComponentCamera : public Component {
public:
    ComponentCamera(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
        : Component(parent, localTransform_, globalTransform) {}
    ComponentCamera(Component *parent, Transformable *globalTransform)
        : Component(parent, globalTransform) {}

    void UpdatePhysic(float deltaTime) {}
    void Render() const override {}

private:
    Camera camera;
};

typedef ComponentCamera C_Camera;

