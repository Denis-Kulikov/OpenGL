#pragma once
#include <object/geometry/geometry.hpp>
#include "../component_dual_quat.hpp"

class ComponentDQShape : public ComponentDualQuat {
public:
    ComponentDQShape(TransformDualQuat *transform)
        : ComponentDualQuat(transform) {}

    void Render() const override;

    GeometryPrimitive *shape;
    Material* material = nullptr;
};
