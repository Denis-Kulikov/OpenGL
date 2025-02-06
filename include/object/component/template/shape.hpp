#pragma once
#include <btBulletDynamicsCommon.h>
#include <lib-project/lib.hpp>
#include <object/sprite.hpp>
#include "../component.hpp"

class ComponentShape : public Component {
public:
    ComponentShape(Component *parent, Transform *transform)
        : Component(parent, transform) {}
    ComponentShape(Component *parent, RigidTransform *transform)
        : Component(parent, transform) {}

    // ComponentShape(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
    //     : Component(parent, localTransform_, globalTransform) {}
    // ComponentShape(Component *parent, Transformable *globalTransform)
    //     : Component(parent, globalTransform) {}

    void Render() const override;

// private:
    Sprite *shape;
};


