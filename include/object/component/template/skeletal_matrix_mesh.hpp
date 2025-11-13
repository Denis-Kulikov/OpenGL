#pragma once
#include "skeletal_mesh.hpp"

class ComponentSkeletalMatrixMesh : public ComponentSkeletalMesh {
public:
    ComponentSkeletalMatrixMesh(const std::string& name, TransformableMatrix *transform)
        : ComponentSkeletalMesh(name, transform) {
        castsShadow = true;
    }
    ComponentSkeletalMatrixMesh(const std::string& name)
        : ComponentSkeletalMesh(name)
    {
        castsShadow = true;
    }

    void Render() const override;
    virtual void RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const override;
    virtual void Update(float deltaTime) override;
    void Accept(UIVisitorComponent& visitor) override { visitor.Visit(*this); }

    std::vector<glm::mat4> boneTransforms;
};
