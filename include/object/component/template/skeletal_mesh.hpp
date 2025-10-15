#pragma once
#include <managers/time.hpp>
#include <object/mesh/skeletal_mesh.hpp>
#include <object/mesh/skeleton/animation/animator.hpp>
#include "../component.hpp"

class ComponentSkeletalMesh : public Component {
public:
    ComponentSkeletalMesh(TransformableMatrix *transform)
        : Component(transform) {}

    void Render() const = 0;
    virtual void Update(float deltaTime) = 0;
    void SetSkeletalMesh(SkeletalMesh *new_mesh);

    Animator* animator = nullptr;
    SkeletalMesh *mesh = nullptr;
};
