#pragma once
#include <lib-project/lib.hpp>
#include <btBulletDynamicsCommon.h>
#include "../../component/transform.hpp"
#include <render/camera.hpp>

class ComponentCamera : public ComponentTransform {
public:
    glm::mat4 UpdateMatrix() { return glm::mat4(); };
    void UpdatePhysic(float deltaTime) {}
    void Render() {}

private:
    Camera camera;
};

typedef ComponentCamera C_Camera;

