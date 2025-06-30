#pragma once
#include <managers/bullet_manager.hpp>
#include <btBulletDynamicsCommon.h>
#include "transformable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class RigidTransform : public Transformable
{
public:
    RigidTransform(btCollisionShape* baseShape, btScalar mass, const glm::vec3& scale);
    ~RigidTransform();

    void UpdateMatrix() override;
    void UpdateTransform() override;
    // void SetMatrix(const glm::mat4x4 &matrix_) override;

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


    void ApplyForce(const glm::vec3& force);
    void ApplyImpulse(const glm::vec3& impulse);
    void ApplyTorque(const glm::vec3& torque);

    glm::vec3 Scale;
    btRigidBody *rigidBody = nullptr;
    btCollisionShape* collisionShape = nullptr;
};
