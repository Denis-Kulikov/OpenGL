#pragma once
#include "../component.hpp"

class ComponentPoint : public Component {
public:
    ComponentPoint(Transform *transform)
        : Component(transform) {}
    // ComponentPoint(RigidTransform *transform)
    //     : Component(transform) {}

    void Render() const override {}
};
