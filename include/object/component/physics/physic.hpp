#pragma once
#include <btBulletDynamicsCommon.h>
#include <lib-project/lib.hpp>
#include <object/sprite.hpp>
#include "../component.hpp"

class ComponentPhysic : public Component {
public:
    // ComponentPhysic(Component *parent, Transform *transform)
    //     : Component(parent, transform) {}
    ComponentPhysic(Component *parent, RigidTransform *transform)
        : Component(parent, transform) {}

    void Render() const override {};

    Sprite *shape;
};


