#pragma once
#include "transformable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class TransformDualQuat : public Transformable
{
public:
    TransformDualQuat();

    glm::quat GetPositionQuat() const;
    glm::vec3 GetPosition() const override;
    glm::quat GetRotation() const override;
    glm::vec3 GetScale() const override;

    void SetPosition(const glm::quat& position);
    void SetPosition(const glm::vec3& position) override;
    void SetRotation(const glm::quat& rotation) override;
    void SetScale(const glm::vec3& scale) override;

    void Move(const glm::vec3& offset) override;
    void Move(glm::vec3 direction, float distance) override;
    void Rotate(const glm::quat& deltaRotation) override;

// protected:
    void moved() {dirty = true;}

    glm::quat dq_real;
    glm::quat dq_dual;
    bool dirty = true;
};
