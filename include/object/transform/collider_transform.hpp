#pragma once
#include <game/bulletManager.hpp>
#include <btBulletDynamicsCommon.h>
#include "transform.hpp"

class ColliderTransform : public Transform
{
public:
    ColliderTransform(BulletManager *btManager, btCollisionShape *colliderShape);
    ~ColliderTransform();

    void UpdateMatrix() override;
    void UpdateTransform() override;
    // void SetMatrix(const glm::mat4x4 &matrix_) override;
    bool isMoving() const override { return false; };

private:
    BulletManager *btManager;
    btCollisionObject *collisionObject;
};
