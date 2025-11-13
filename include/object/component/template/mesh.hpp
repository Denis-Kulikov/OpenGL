#pragma once
#include <object/mesh/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(const std::string& name, TransformableMatrix *transform)
        : Component(name, transform) {
        castsShadow = true;
    }
    ComponentMesh(const std::string& name)
        : Component(name)
    {
        castsShadow = true;
    }

    void SetMesh(Mesh* new_mesh);
    void Render() const override;
    virtual void RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const override;
    void Accept(UIVisitorComponent& visitor) override { visitor.Visit(*this); }

    Mesh *mesh = nullptr;
};
