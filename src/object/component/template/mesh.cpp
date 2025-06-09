#include <managers/render_manager.hpp>
#include <object/component/template/mesh.hpp>

void ComponentMesh::Render() const {
    material->UpdateValue("Projection", RenderManager::render.ProjTrans);
    material->UpdateValue("View", RenderManager::render.View);
    material->UpdateValue("Model", GetMatrix());

    material->Bind();
    mesh->Bind();
    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }
}
