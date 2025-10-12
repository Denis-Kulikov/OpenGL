#pragma once
#include <managers/time_manager.hpp>
#include <object/mesh/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(TransformableMatrix *transform)
        : Component(transform) {}

    void Render() const override;

    Mesh *mesh = nullptr;
};
