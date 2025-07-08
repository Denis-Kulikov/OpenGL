#pragma once
#include "../component.hpp"

class ComponentPoint : public Component {
public:
    ComponentPoint(TransformableMatrix *transform)
        : Component(transform) {}
};
