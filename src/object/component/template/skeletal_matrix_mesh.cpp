#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <managers/render/render.hpp>

void PrintMatrix(const glm::mat4& matrix);

void ComponentSkeletalMatrixMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    // std::cout << "boneTransforms: " << boneTransforms.size() << std::endl; 
    RenderManager::bufferManager.UpdateBonesDataSSBO(boneTransforms);
    mesh->material.Set("Model", model_mats4x4);
    // mesh->material.Set("gBones", boneTransforms);
    mesh->Bind();

    // auto& m = mesh->m_Entries[0];
    for (const auto& m : mesh->m_Entries) {
        m.material.Bind(mesh->shader);
        m.Draw();
    }
}

void ComponentSkeletalMatrixMesh::Update(float deltaTime) {
    animator->ApplyAnimation(boneTransforms, deltaTime);
}
