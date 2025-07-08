#include <managers/render_manager.hpp>
#include <object/component/template/shape.hpp>

void ComponentShape::Render() const {
    glm::mat4x3 mat4x3 = globalTransform->GetMatrix();

    glm::mat4 mat4 = glm::mat4(
        glm::vec4(mat4x3[0], 0.0f),
        glm::vec4(mat4x3[1], 0.0f),
        glm::vec4(mat4x3[2], 0.0f),
        glm::vec4(mat4x3[3], 1.0f)
    );

    auto mat4x4 = RenderManager::pipeline.PV * mat4;
    material->UpdateValue("gWorld", mat4x4);
    material->Bind();
    if (!material->GetTexture().empty())
        material->GetTexture()[0]->Bind();
    shape->Bind();
    shape->Draw();
}
