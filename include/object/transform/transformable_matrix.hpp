#pragma once
#include "transformable.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

class TransformableMatrix : public Transformable {
public:
    TransformableMatrix(const glm::mat4x3& mat) { matrix = mat; }
    TransformableMatrix() {}
    const glm::mat4x3 &GetMatrix() const { return matrix; };
    virtual void UpdateMatrix() = 0;
    virtual void UpdateTransform() = 0;
    virtual void SetMatrix(const glm::mat4x3& mat) { matrix = mat; };

    virtual void RotateAround(const glm::vec3& axis, float angle) = 0;

    virtual glm::vec3 GetForwardVector() const = 0;
    virtual glm::vec3 GetUpVector() const = 0;
    virtual glm::vec3 GetRightVector() const = 0;

protected:
    glm::mat4x3 matrix;
};
