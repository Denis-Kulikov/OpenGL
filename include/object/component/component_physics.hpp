#pragma once
#include "component.hpp"

class ComponentPhysics : public Component {
public:
    ComponentPhysics(RigidTransform *transform);

    // virtual void UpdateInverse() override;
    // virtual void UpdateMatrix() override;
    // virtual void UpdateMatrixTree(const glm::mat4x4& parentTR = glm::mat4(1.0f), const glm::mat4x4& parentS = glm::mat4(1.0f)) override;
    void Render() const override {};

    bool fIsInit = 0;
};
