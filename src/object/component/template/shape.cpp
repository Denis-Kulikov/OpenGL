#include <managers/render_manager.hpp>
#include <object/component/template/shape.hpp>
#include <object/geometry/mesh.hpp>

void ComponentShape::Render() const {
    auto mat4x4 = RenderManager::render.GetPV() * globalTransform->GetMatrix();
    material->UpdateValue("gWorld", mat4x4);

    switch (shape->GetType()) {
    case GeometryBase::Type::PRIMITIVE:
        material->Bind();
        if (!material->GetTexture().empty())
            material->GetTexture()[0]->Bind();
        shape->Bind();
        shape->Draw();
        break;
    
    case GeometryBase::Type::MESH:
        GeometryMesh* gMesh = static_cast<GeometryMesh*>(shape);
        material->Bind();
        gMesh->Bind();
        for (int i = 0; i < gMesh->size(); ++i) {
            if (!material->GetTexture().empty()) {
                int index = gMesh->GetTextureIndex(i);
                material->GetTexture()[index]->Bind();
            }
            gMesh->Draw(&i);
        }
        break;
    }
}
