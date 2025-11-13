#pragma once
#include <managers/time.hpp>
#include <object/mesh/skeletal_mesh.hpp>
#include <object/mesh/skeleton/animation/animator.hpp>
#include "../component.hpp"

class ComponentSkeletalMesh : public Component {
public:
    ComponentSkeletalMesh(const std::string& name, TransformableMatrix *transform)
        : Component(name, transform) {
        castsShadow = true;
    }
    ComponentSkeletalMesh(const std::string& name)
        : Component(name)
    {
        castsShadow = true;
    }

    void Render() const = 0;
    virtual void Update(float deltaTime) = 0;
    void SetSkeletalMesh(SkeletalMesh *new_mesh);

    Animator* animator = nullptr;
    SkeletalMesh *mesh = nullptr;
};
