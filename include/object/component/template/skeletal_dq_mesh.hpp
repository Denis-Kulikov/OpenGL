#pragma once
#include "skeletal_mesh.hpp"

class ComponentSkeletalDQMesh : public ComponentSkeletalMesh {
public:
    ComponentSkeletalDQMesh(TransformableMatrix *transform)
        : ComponentSkeletalMesh(transform) {}

    void Render() const override;
    virtual void Update(float deltaTime) override;

    std::vector<glm::dualquat> boneTransforms;
};
