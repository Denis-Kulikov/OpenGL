#pragma once
#include <object/geometry/geometry.hpp>
#include "../component.hpp"

class ComponentShape : public Component {
public:
    ComponentShape(TransformableMatrix *transform)
        : Component(transform) {}

    void Render() const override;

    GeometryPrimitive *shape;
    Material* material = nullptr;
};
