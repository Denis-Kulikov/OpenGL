#include <managers/render/render.hpp>
#include <object/component/template/mesh.hpp>
#include <scene/shadow/shadow_map.hpp>
#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <managers/global.hpp> 
#include <scene/scene.hpp>


void ComponentMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material->Set("Model", model_mats4x4);
    mesh->material->Set("dirLightSpaceMatrix", GlobalState::scene->shadow.directionalLight.lightSpaceMatrix);

    mesh->material->Set("hasSpecularMap", bool(false));
    mesh->material->Set("roughness", float(0.65f));
    mesh->material->Set("metallic", float(0.01f));
    mesh->material->Set("ambientStrength", float(0.375f));
    mesh->material->Set("specularStrength", float(0.05f));

    mesh->material->Set("lightPos", GlobalState::scene->shadow.pointLights.GetPosition());
    mesh->material->Set("farPlanes", GlobalState::scene->shadow.pointLights.GetFar());
    mesh->material->Set("nearPlanes", GlobalState::scene->shadow.pointLights.GetNear());
    
    mesh->Bind();

    for (const auto& m : mesh->m_Entries) {
        m.material->Bind(mesh->shader);
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
        mesh->material->BindShadowPass(shader, model_mats4x4, shadowProj);
        mesh->BindGeometry();

        for (const auto& m : mesh->m_Entries)
            m.Draw();
    }
}