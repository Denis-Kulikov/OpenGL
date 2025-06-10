#include <managers/bullet_manager.hpp>

void BulletManager::Initialize() {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

void BulletManager::Dispose() {
    delete dynamicsWorld;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfiguration;
}


void BulletManager::AddRigidBody(btRigidBody* body) {
    dynamicsWorld->addRigidBody(body);
}

void BulletManager::RemoveRigidBody(btRigidBody* body) {
    dynamicsWorld->removeRigidBody(body);
    delete body;
}

void BulletManager::AddCollisionObject(btCollisionObject* collisionObject) {
    dynamicsWorld->addCollisionObject(collisionObject);
}

void BulletManager::RemoveCollisionObject(btCollisionObject* collisionObject) {
    dynamicsWorld->removeCollisionObject(collisionObject);
    delete collisionObject;
}

void BulletManager::StepSimulation(float deltaTime) {
    dynamicsWorld->stepSimulation(deltaTime, 10);
}
