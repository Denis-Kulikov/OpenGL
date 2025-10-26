#pragma once
#include "component.hpp"

class ComponentPhysics : public Component {
public:
    ComponentPhysics(RigidTransform *transform);

    void Render() const override {};
};
