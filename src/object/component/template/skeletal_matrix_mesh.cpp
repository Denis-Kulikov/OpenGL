#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>

glm::mat4 dq2mat(const glm::dualquat& dq) {
    glm::dualquat normDQ = glm::normalize(dq);

    glm::quat rotation = normDQ.real;

    glm::quat t_quat = normDQ.dual * glm::conjugate(normDQ.real);
    glm::vec3 translation = 2.0f * glm::vec3(t_quat.x, t_quat.y, t_quat.z);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 R = glm::mat4_cast(rotation);

    return T * R;
}


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
