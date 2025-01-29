#pragma once
#include <lib-project/lib.hpp>
#include <game/bulletManager.hpp>
#include <btBulletDynamicsCommon.h>
#include "transform.hpp"

class ColliderTransform : public Transform
{
public:
    ColliderTransform(BulletManager *btManager, btCollisionShape *colliderShape);
    ~ColliderTransform();

    void UpdateMatrix() override;
    void SetMatrix(const glm::mat4x4 &matrix_) override;
    bool isMoving() const override { return false; };

private:
    void UpdateTransform() override;
    BulletManager *btManager;
    btCollisionObject *collisionObject;
};
