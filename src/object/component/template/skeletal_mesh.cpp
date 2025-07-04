#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_mesh.hpp>

void ComponentSkeletalMesh::Render() const {
    // const auto it = material->values.find("gBones");
    // if (it == material->values.end()) {
    //     return;
    // }
    // int boneLocation = it->second.first;

    const auto q_real_it = material->values.find("gBoneRotations");
    const auto q_dual_it = material->values.find("gBoneTranslations");
    if (q_real_it == material->values.end() || q_dual_it == material->values.end()) {
        std::cout << "Error (ComponentSkeletalMesh::Render): \
                      q_real_it == material->values.end() || q_dual_it == material->values.end()" << std::endl;
        return;
    }
    int q_real = q_real_it->second.first;
    int q_dual = q_dual_it->second.first;

    material->UpdateValue("Projection", RenderManager::pipeline.ProjTrans);
    material->UpdateValue("View", RenderManager::pipeline.View);

    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );
    material->UpdateValue("Model", model_mats4x4);
    material->Bind();
    mesh->Bind();

    // glUniformMatrix4x3fv(boneLocation, boneTransforms.size(), GL_FALSE, glm::value_ptr(boneTransforms[0]));

    // std::vector<glm::mat4x4> mats4x4(boneTransforms.size());
    // for (int i = 0; i < boneTransforms.size(); ++i) {
    //     mats4x4[i] = glm::mat4(
    //         glm::vec4(boneTransforms[i][0], 0.0f),
    //         glm::vec4(boneTransforms[i][1], 0.0f),
    //         glm::vec4(boneTransforms[i][2], 0.0f),
    //         glm::vec4(boneTransforms[i][3], 1.0f)
    //     );
    // }
    // glUniformMatrix4fv(boneLocation, mats4x4.size(), GL_FALSE, glm::value_ptr(mats4x4[0]));

    glUniform4fv(q_real, boneRotations.size(), glm::value_ptr(boneRotations[0]));
    glUniform4fv(q_dual, boneTranslations.size(), glm::value_ptr(boneTranslations[0]));

    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }
}

void ComponentSkeletalMesh::Update(float deltaTime) {
    // animator->ApplyAnimation(boneTransforms, deltaTime);
    animator->ApplyAnimationDQ(boneRotations, boneTranslations, deltaTime);
}

void ComponentSkeletalMesh::SetSkeletalMesh(GeometrySkeletalMesh *new_mesh) {
    if (new_mesh != nullptr) {
        mesh = new_mesh;
        animator = new Animator(mesh->GetSkeleton());
    }
}
