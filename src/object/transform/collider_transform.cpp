#include <object/transform/collider_transform.hpp>

ColliderTransform::ColliderTransform(BulletManager *btManager, btCollisionShape *colliderShape)
    : btManager(btManager)
{
    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(colliderShape);
    btManager->AddCollisionObject(collisionObject);
    void AddCollisionObject(btCollisionObject* collisionObject);
}

ColliderTransform::~ColliderTransform() {
    btManager->RemoveCollisionObject(collisionObject);
    delete collisionObject;
}


void ColliderTransform::UpdateMatrix() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), ColliderTransform::GetPosition());
    glm::mat4 rotate = glm::mat4_cast(ColliderTransform::GetRotation());
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), ColliderTransform::GetScale());
    matrix = translate * rotate * scale;
}
