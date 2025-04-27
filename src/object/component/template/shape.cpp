#include <game/gameManager.hpp>
#include <object/component/template/shape.hpp>

void ComponentShape::Render() const {
    Sprite::Sprite_rdata rdata = { GameManager::render.GetPV_Orthographic() * globalTransform->GetMatrix() };
    shape->Render(&rdata);
}