#include <managers/render/render.hpp>
#include <object/component/template/mesh.hpp>

void ComponentMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material.Set("Model", model_mats4x4);
    mesh->Bind();

    for (const auto& m : mesh->m_Entries) {
        m.Material.Bind(mesh->shader);
        m.Draw();
    }
}
