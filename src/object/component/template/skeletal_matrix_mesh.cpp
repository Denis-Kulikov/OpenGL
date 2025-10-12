#include <object/component/template/skeletal_matrix_mesh.hpp>

void PrintMatrix(const glm::mat4& matrix);

void ComponentSkeletalMatrixMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material.Set("Model", model_mats4x4);
    mesh->material.Set("gBones", boneTransforms);
    mesh->Bind();

    // static bool f = true;
    // if (f)
    //     for (auto& it : boneTransforms) {
    //         PrintMatrix(it);
    //         std::cout << std::endl;
    //     }
    // f = false;

    for (const auto& m : mesh->m_Entries) {
        m.Material.Bind(mesh->shader);
        m.Draw();
    }
}

void ComponentSkeletalMatrixMesh::Update(float deltaTime) {
    animator->ApplyAnimation(boneTransforms, deltaTime);
}
