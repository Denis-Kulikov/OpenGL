#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>

void ComponentSkeletalDQMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    material->Set("Model", model_mats4x4);
    material->Set("DQ", boneTransforms);
    material->Bind();
    mesh->Bind();

    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }
}

void ComponentSkeletalDQMesh::Update(float deltaTime) {
    animator->ApplyAnimationDQ(boneTransforms, deltaTime);
}
