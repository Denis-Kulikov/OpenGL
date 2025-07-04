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
    virtual void Update(float deltaTime) override;
    void SetSkeletalMesh(GeometrySkeletalMesh *new_mesh);

    Animator* animator;
    GeometrySkeletalMesh *mesh;
    Material* material = nullptr;
    std::vector<glm::mat4x3> boneTransforms;
    std::vector<glm::quat> boneRotations;     // q_real
    std::vector<glm::quat> boneTranslations;  // q_dual
};
