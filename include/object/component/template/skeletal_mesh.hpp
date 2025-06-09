#pragma once
#include <managers/time_manager.hpp>
#include <object/geometry/skeletal_mesh.hpp>
#include <object/geometry/skeleton/animation/animator.hpp>
#include "../component.hpp"

class ComponentSkeletalMesh : public Component {
public:
    ComponentSkeletalMesh(Transform *transform)
        : Component(transform) {}

    void UpdateInverse() override;
    void UpdateMatrix() override;
    void Render() const override;
    void SetSkeletalMesh(GeometrySkeletalMesh *new_mesh);
    void AttachToBone(int boneIndex, Component* component);

    std::vector<std::pair<int, Component*>> attachedToBone;
    std::vector<glm::mat4x4> mats;
    Animator* animator;
    GeometrySkeletalMesh *mesh;
};
