#pragma once
#include "component.hpp"

class ComponentPhysics : public Component {
public:
    ComponentPhysics(const std::string& name, RigidTransform *transform);

    void Accept(UIVisitorComponent& visitor) override { visitor.Visit(*this); }
};
