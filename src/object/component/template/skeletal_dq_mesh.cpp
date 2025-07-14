#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>

void ComponentSkeletalDQMesh::Render() const {
    const auto it_dq = material->values.find("gDQ");
    if (it_dq == material->values.end()) {
        std::cout << "Error (ComponentSkeletalDQMesh::Render): it_dq == material->values.end()" << std::endl;
        return;
    }
    int loc_dq = it_dq->second.first;

    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    debug->rootDualQuat->globalTransform->dq = boneTransforms[0];
    debug->rootDualQuat->children[0]->globalTransform->dq = boneTransforms[1];
    debug->rootDualQuat->children[0]->children[0]->globalTransform->dq = boneTransforms[2];
    // debug->rootDualQuat->RenderTree();

    material->UpdateValue("Projection", RenderManager::pipeline.ProjTrans);
    material->UpdateValue("View", RenderManager::pipeline.View);
    material->UpdateValue("Model", model_mats4x4);
    material->Bind();
    glUniform4fv(loc_dq, boneTransforms.size() * 2, glm::value_ptr(boneTransforms.data()[0].real));
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
