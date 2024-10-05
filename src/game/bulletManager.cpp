#include <game/bulletManager.hpp>

BulletManager::BulletManager ()
: broadphase(), 
  collisionConfiguration(), 
  dispatcher(&collisionConfiguration),
  solver(), 
  dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration) 
{
    dynamicsWorld.setGravity(btVector3(0, -9.81f, 0));
}

// BulletManager::~BulletManager () {
// }
