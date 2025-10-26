#pragma once
#include <object/mesh/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(TransformableMatrix *transform)
        : Component(transform) {}

    void Render() const override;
    virtual void RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const override;

    Mesh *mesh = nullptr;
};
