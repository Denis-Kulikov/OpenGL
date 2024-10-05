#include <btBulletDynamicsCommon.h>

class BulletManager {
public:
    BulletManager();
    ~BulletManager() = default;

private:
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;
    btDiscreteDynamicsWorld dynamicsWorld;
};
