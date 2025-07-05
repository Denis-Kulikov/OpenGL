#pragma once
#include <managers/time_manager.hpp>
#include <object/geometry/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(TransformableMatrix *transform)
        : Component(transform) {}

    void Render() const override;

    GeometryMesh *mesh;
    Material* material = nullptr;
};
