#include <object/transform/rigid_transform.hpp>

RigidTransform::RigidTransform(btCollisionShape* baseShape, btScalar mass, const glm::vec3& scale)
    : Scale(scale)
{
    btVector3 localInertia(0, 0, 0);
    if (mass != 0.f && baseShape)
        baseShape->calculateLocalInertia(mass, localInertia);

    if (baseShape)
        baseShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

    btDefaultMotionState* motionState = new btDefaultMotionState();

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, baseShape, localInertia);
    rigidBody = new btRigidBody(rbInfo);
    BulletManager::AddRigidBody(rigidBody);
}

RigidTransform::RigidTransform(const RigidTransform &other)
    : Scale(other.Scale) {
    if (other.rigidBody) {
        btCollisionShape *shape = other.rigidBody->getCollisionShape();
        btScalar mass = 1.0f / other.rigidBody->getInvMass();

        btVector3 localInertia(0, 0, 0);
        if (mass > 0.0f) {
            shape->calculateLocalInertia(mass, localInertia);
        }

        btTransform initialTransform;
        other.rigidBody->getMotionState()->getWorldTransform(initialTransform);
        btMotionState *motionState = new btDefaultMotionState(initialTransform);

        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
            mass, motionState, shape, localInertia);
        rigidBody = new btRigidBody(rigidBodyCI);

        BulletManager::AddRigidBody(rigidBody);
    }
}

RigidTransform &RigidTransform::operator=(const RigidTransform &other) {
    if (this == &other) return *this;

    if (rigidBody) {
        BulletManager::RemoveRigidBody(rigidBody);
        delete rigidBody->getMotionState();
        delete rigidBody;
    }

    Scale = other.Scale;

    if (other.rigidBody) {
        btCollisionShape *shape = other.rigidBody->getCollisionShape();
        btScalar mass = 1.0f / other.rigidBody->getInvMass(); 

        btVector3 localInertia(0, 0, 0);
        if (mass > 0.0f) {
            shape->calculateLocalInertia(mass, localInertia);
        }

        btTransform initialTransform;
        other.rigidBody->getMotionState()->getWorldTransform(initialTransform);
        btMotionState *motionState = new btDefaultMotionState(initialTransform);

        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
            mass, motionState, shape, localInertia);
        rigidBody = new btRigidBody(rigidBodyCI);

        BulletManager::AddRigidBody(rigidBody);
    }

    return *this;
}

RigidTransform::~RigidTransform()
{
    BulletManager::RemoveRigidBody(rigidBody);
    delete rigidBody->getMotionState(); 
    delete rigidBody;
}

void RigidTransform::UpdateTransform() {
    RigidTransform::SetPosition(glm::vec3(matrix[3]));
    RigidTransform::SetRotation(glm::quat_cast(matrix));
    RigidTransform::SetScale(glm::vec3(glm::length(matrix[0]),
                        glm::length(matrix[1]),
                        glm::length(matrix[2])));
}

void RigidTransform::UpdateMatrix() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), RigidTransform::GetPosition());
    glm::mat4 rotate = glm::mat4_cast(RigidTransform::GetRotation());
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), RigidTransform::GetScale());
    matrix = translate * rotate * scale;
}


glm::vec3 RigidTransform::GetPosition() const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 pos = transform.getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

glm::quat RigidTransform::GetRotation() const
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion rot = transform.getRotation();
    return glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
}

glm::vec3 RigidTransform::GetScale() const
{
    return Scale;
}

void RigidTransform::SetScale(const glm::vec3& scale)
{
    Scale = scale;
}

void RigidTransform::SetPosition(const glm::vec3& position)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    rigidBody->getMotionState()->setWorldTransform(transform);
    rigidBody->setWorldTransform(transform);
}

void RigidTransform::SetRotation(const glm::quat& rotation)
{
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    rigidBody->getMotionState()->setWorldTransform(transform);
    rigidBody->setWorldTransform(transform);
}

void RigidTransform::Move(const glm::vec3& offset)
{
    glm::vec3 currentPos = GetPosition();
    SetPosition(currentPos + offset);
}

void RigidTransform::Move(glm::vec3 direction, float distance)
{
    if (glm::length(direction) != 0) {
        direction = glm::normalize(direction);
        Move(direction * distance);
    }
}

void RigidTransform::Rotate(const glm::quat& deltaRotation)
{
    glm::quat currentRotation = GetRotation();
    SetRotation(currentRotation * deltaRotation);
}

void RigidTransform::RotateAround(const glm::vec3& axis, float angle)
{
    glm::quat delta = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    Rotate(delta);
}

glm::vec3 RigidTransform::GetForwardVector() const
{
    return glm::normalize(GetRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 RigidTransform::GetUpVector() const
{
    return glm::normalize(GetRotation() * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 RigidTransform::GetRightVector() const
{
    return glm::normalize(GetRotation() * glm::vec3(1.0f, 0.0f, 0.0f));
}

void RigidTransform::ApplyForce(const glm::vec3& force) { rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z)); }
void RigidTransform::ApplyImpulse(const glm::vec3& impulse) { rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z)); }
void RigidTransform::ApplyTorque(const glm::vec3& torque) { rigidBody->applyTorque(btVector3(torque.x, torque.y, torque.z)); }
