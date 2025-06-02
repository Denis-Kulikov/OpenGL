#pragma once
#include "transformable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform : public Transformable
{
public:
    Transform(const Transform &transform);
    Transform();

    void UpdateMatrix() override;
    void UpdateTransform() override;
    Transform& operator=(const glm::mat4x4& matrix_) {
        matrix = matrix_;
        UpdateTransform();
        return *this;
    }
    Transform& operator=(const Transformable& transform) {
        *this = transform.GetMatrix();
        return *this;
    }
    bool isMoving() const override { return false; };

    glm::vec3 GetPosition() const override;
    glm::quat GetRotation() const override;
    glm::vec3 GetScale() const override;

    void SetPosition(const glm::vec3& position) override;
    void SetRotation(const glm::quat& rotation) override;
    void SetScale(const glm::vec3& scale) override;

    void Move(const glm::vec3& offset) override;
    void Move(glm::vec3 direction, float distance) override;
    void Rotate(const glm::quat& deltaRotation) override;
    void RotateAround(const glm::vec3& axis, float angle) override;

    glm::vec3 GetForwardVector() const override;
    glm::vec3 GetUpVector() const override;
    glm::vec3 GetRightVector() const override;

private:
    glm::vec3 Position;
    glm::quat Rotation;
    glm::vec3 Scale;
};
