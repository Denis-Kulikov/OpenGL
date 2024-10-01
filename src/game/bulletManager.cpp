#include <game/bulletManager.hpp>

BulletManager::BulletManager () {
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

BulletManager::~BulletManager () {
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    delete dynamicsWorld;
}
