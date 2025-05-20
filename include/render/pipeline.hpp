#pragma once
#include "camera.hpp"
#include "math/Matrix4f.hpp"
#include "../object/transform/transform.hpp"

class Pipeline
{
public:
    Pipeline() {}

    glm::mat4 GetModel(const objectTransform& transform) const;
    glm::mat4 GetModel(const Transform& transform) const;

    Camera camera;
};
