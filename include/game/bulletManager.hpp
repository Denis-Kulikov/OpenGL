#pragma once
#include <btBulletDynamicsCommon.h>

class BulletManager {
public:
    BulletManager();
    ~BulletManager();

    void AddRigidBody(btRigidBody* body);
    void RemoveRigidBody(btRigidBody* body);

    void AddCollisionObject(btCollisionObject* collisionObject);
    void RemoveCollisionObject(btCollisionObject* collisionObject);
    
    void StepSimulation(float deltaTime);

// private:
    btBroadphaseInterface* broadphase = nullptr;
    btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
};
