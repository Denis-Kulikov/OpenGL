#pragma once
#include <btBulletDynamicsCommon.h>
#include <object/sprite.hpp>
#include "../component.hpp"

class ComponentPhysic : public Component {
public:
    // ComponentPhysic(Component *parent, Transform *transform)
    //     : Component(parent, transform) {}
    ComponentPhysic(RigidTransform *transform)
        : Component(transform) {}

    void Render() const override {};

    Sprite *shape;
};


