#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>

void ComponentSkeletalMatrixMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    material->UpdateValue("Projection", RenderManager::pipeline.ProjTrans);
    material->UpdateValue("View", RenderManager::pipeline.View);
    material->UpdateValue("Model", model_mats4x4);
    material->Bind();

    const auto it_mats = material->values.find("gBones");
    glUniformMatrix4fv(it_mats->second.first, boneTransforms.size(), GL_FALSE, glm::value_ptr(*boneTransforms.data()));
    mesh->Bind();

    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }
}

void ComponentSkeletalMatrixMesh::Update(float deltaTime) {
    animator->ApplyAnimation(boneTransforms, deltaTime);
}
