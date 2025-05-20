#include <managers/render_manager.hpp>
#include <object/component/template/shape.hpp>

void ComponentShape::Render() const {
    Sprite::Sprite_rdata rdata = { RenderManager::render.GetPV() * globalTransform->GetMatrix() };
    shape->Render(&rdata);
}