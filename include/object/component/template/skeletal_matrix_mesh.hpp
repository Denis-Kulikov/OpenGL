#pragma once
#include "skeletal_mesh.hpp"

class ComponentSkeletalMatrixMesh : public ComponentSkeletalMesh {
public:
    ComponentSkeletalMatrixMesh(TransformableMatrix *transform)
        : ComponentSkeletalMesh(transform) {}

    void Render() const override;
    virtual void Update(float deltaTime) override;

    std::vector<glm::mat4> boneTransforms;
};
