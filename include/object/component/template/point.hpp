#pragma once
#include <managers/render_manager.hpp>
#include <mesh/mesh.hpp>
#include "../component.hpp"

class ComponentPoint : public Component {
public:
    ComponentPoint(Transform *transform)
        : Component(transform) {}
    // ComponentPoint(RigidTransform *transform)
    //     : Component(transform) {}

    void Render() const override {}
};
