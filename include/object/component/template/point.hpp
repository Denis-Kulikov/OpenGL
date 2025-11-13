#pragma once
#include "../component.hpp"

class ComponentPoint : public Component {
public:
    ComponentPoint(const std::string& name, TransformableMatrix *transform)
        : Component(name, transform) {}
    ComponentPoint(const std::string& name)
        : Component(name)
    {}

    void Accept(UIVisitorComponent& visitor) override { visitor.Visit(*this); }
};
