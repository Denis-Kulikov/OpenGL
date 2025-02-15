#pragma once
#include <object/sprite.hpp>
#include "../component.hpp"

class ComponentShape : public Component {
public:
    ComponentShape(Transform *transform)
        : Component(transform) {}
    // ComponentShape(RigidTransform *transform)
    //     : Component(transform) {}

    // ComponentShape(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
    //     : Component(parent, localTransform_, globalTransform) {}
    // ComponentShape(Component *parent, Transformable *globalTransform)
    //     : Component(parent, globalTransform) {}

    void Render() const override;

// private:
    Sprite *shape;
};


