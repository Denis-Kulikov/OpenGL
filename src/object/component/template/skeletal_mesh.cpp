#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_mesh.hpp>

void ComponentSkeletalMesh::Render() const {
    // const auto it = material->values.find("gBones");
    // if (it == material->values.end()) {
    //     return;
    // }
    // int boneLocation = it->second.first;

    const auto it_dq = material->values.find("gDQ");
    if (it_dq == material->values.end()) {
        std::cout << "Error (ComponentSkeletalMesh::Render): it_dq == material->values.end()" << std::endl;
        return;
    }
    int loc_dq = it_dq->second.first;

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

    glUniform4fv(loc_dq, boneTransforms.size() * 2, glm::value_ptr(boneTransforms.data()[0].real));

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
    animator->ApplyAnimationDQ(boneTransforms, deltaTime);
}

void ComponentSkeletalMesh::SetSkeletalMesh(GeometrySkeletalMesh *new_mesh) {
    if (new_mesh != nullptr) {
        mesh = new_mesh;
        animator = new Animator(mesh->GetSkeleton());
    }
}
