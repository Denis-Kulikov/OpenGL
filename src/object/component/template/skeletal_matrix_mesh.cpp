#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <managers/render/render.hpp>
#include <object/material/material.hpp>
#include <object/material/shader.hpp>


void ComponentSkeletalMatrixMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    RenderManager::bufferManager.UpdateBonesDataSSBO(boneTransforms);
    mesh->material->Set("Model", model_mats4x4);
    // mesh->material->Set("gBones", boneTransforms);
    mesh->Bind();

    // auto& m = mesh->m_Entries[0];
    for (const auto& m : mesh->m_Entries) {
        m.material->Bind(mesh->shader);
        m.Draw();
    }
}

void ComponentSkeletalMatrixMesh::RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const {
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

        mesh->material->Set("ShadowProj", shadowProj);
        mesh->material->Set("Model", model_mats4x4);
        mesh->material->Bind(shader);

        for (const auto& m : mesh->m_Entries)
            m.Draw();
    }
}

void ComponentSkeletalMatrixMesh::Update(float deltaTime) {
    animator->ApplyAnimation(boneTransforms, deltaTime);
}
