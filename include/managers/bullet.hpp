#pragma once
#include <btBulletDynamicsCommon.h>

class BulletManager {
public:
    static void Initialize();
    static void Dispose();

    static void AddRigidBody(btRigidBody* body);
    static void RemoveRigidBody(btRigidBody* body);

    static void AddCollisionObject(btCollisionObject* collisionObject);
    static void RemoveCollisionObject(btCollisionObject* collisionObject);
    
    static void UpdateSingleAabb(btRigidBody* body);

    static void StepSimulation(float deltaTime);

private:
    inline static btBroadphaseInterface* broadphase = nullptr;
    inline static btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
    inline static btCollisionDispatcher* dispatcher = nullptr;
    inline static btSequentialImpulseConstraintSolver* solver = nullptr;
    inline static btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
};
