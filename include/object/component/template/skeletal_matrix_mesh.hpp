#pragma once
#include "skeletal_mesh.hpp"

class ComponentSkeletalMatrixMesh : public ComponentSkeletalMesh {
public:
    ComponentSkeletalMatrixMesh(TransformableMatrix *transform)
        : ComponentSkeletalMesh(transform) {
            boneTransforms.reserve(128);
            for (int i = 0; i < 128; ++i){
                boneTransforms.push_back(glm::mat4(1.f));
            }
        }

    void Render() const override;
    virtual void Update(float deltaTime) override;

    std::vector<glm::mat4> boneTransforms;
};
