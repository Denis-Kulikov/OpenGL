#include <managers/render/render.hpp>
#include <object/component/template/mesh.hpp>
#include <scene/shadow/shadow_map.hpp>

void ComponentMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material.Set("Model", model_mats4x4);
    mesh->material.Set("hasSpecularMap", bool(false));
    mesh->material.Set("roughness", float(0.85f));
    mesh->material.Set("metallic", float(0.1f));
    mesh->material.Set("ambientStrength", float(0.25f));
    mesh->material.Set("specularStrength", float(0.05f));
    
    mesh->Bind();

    for (const auto& m : mesh->m_Entries) {
        m.material.Bind(mesh->shader);
        m.Draw();
    }
}

void ComponentMesh::RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const {
    if (castsShadow && mesh != nullptr) {
        Shader* shader = nullptr;
        shader = Shader::Find("ShadowMap");

        Material material;

        auto model_mats4x4 = glm::mat4(
            glm::vec4(GetMatrix()[0], 0.0f),
            glm::vec4(GetMatrix()[1], 0.0f),
            glm::vec4(GetMatrix()[2], 0.0f),
            glm::vec4(GetMatrix()[3], 1.0f)
        );

        shader->Bind();

        material.Set("ShadowProj", shadowProj);
        material.Set("Model", model_mats4x4);
        material.Bind(shader);

        mesh->BindGeometry();

        for (const auto& m : mesh->m_Entries)
            m.Draw();
    }
}