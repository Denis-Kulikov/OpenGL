#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

class Transformable {
public:
    Transformable(const glm::mat4x3& mat) { matrix = mat; }
    Transformable() {}
    const glm::mat4x3 &GetMatrix() const { return matrix; };
    virtual void UpdateMatrix() = 0;
    virtual void UpdateTransform() = 0;

    virtual glm::vec3 GetPosition() const = 0;
    virtual glm::quat GetRotation() const = 0;
    virtual glm::vec3 GetScale() const = 0;


    virtual void SetMatrix(const glm::mat4x3& mat) { matrix = mat; };

    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual void SetRotation(const glm::quat& rotation) = 0;
    virtual void SetScale(const glm::vec3& scale) = 0;

    virtual void Move(const glm::vec3& offset) = 0;
    virtual void Move(glm::vec3 direction, float distance) = 0;
    virtual void Rotate(const glm::quat& deltaRotation) = 0;
    virtual void RotateAround(const glm::vec3& axis, float angle) = 0;

    virtual glm::vec3 GetForwardVector() const = 0;
    virtual glm::vec3 GetUpVector() const = 0;
    virtual glm::vec3 GetRightVector() const = 0;

protected:
    glm::mat4x3 matrix;
};
