#pragma once
#include <managers/time_manager.hpp>
#include <object/geometry/skeletal_mesh.hpp>
#include <object/geometry/skeleton/animation/animator.hpp>
#include "../component.hpp"

class ComponentSkeletalMesh : public Component {
public:
    ComponentSkeletalMesh(Transform *transform)
        : Component(transform) {}

    void Render() const override;
    void SetSkeletalMesh(GeometrySkeletalMesh *new_mesh);

    Animator* animator;
    GeometrySkeletalMesh *mesh;
};
