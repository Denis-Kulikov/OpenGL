#include <managers/render_manager.hpp>
#include <object/component/template/shape.hpp>

void ComponentShape::Render() const {
    auto mat4x4 = RenderManager::render.GetPV() * globalTransform->GetMatrix();
    material->UpdateValue("gWorld", mat4x4);
    material->Bind();
    if (!material->GetTexture().empty())
        material->GetTexture()[0]->Bind();
    shape->Bind();
    shape->Draw();
}
