#include <object/component/template/skeletal_dq_mesh.hpp>

void ComponentSkeletalDQMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material.Set("Model", model_mats4x4);
    mesh->material.Set("DQ", boneTransforms);
    mesh->material.Bind(mesh->shader);
    mesh->Bind();

    for (int i = 0; i < mesh->m_Entries.size(); ++i) {
        mesh->m_Entries[i].material.Bind(mesh->shader);
        mesh->m_Entries[i].Draw();
    }
}

void ComponentSkeletalDQMesh::RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const {
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

        for (const auto& m : mesh->m_Entries)
            m.Draw();
    }
}

void ComponentSkeletalDQMesh::Update(float deltaTime) {
    animator->ApplyAnimationDQ(boneTransforms, deltaTime);
}
