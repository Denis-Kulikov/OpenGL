#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_mesh.hpp>

void ComponentSkeletalMesh::Render() const {
    const auto it = material->values.find("gBones");
    if (it == material->values.end()) {
        return;
    }

    material->UpdateValue("Projection", RenderManager::pipeline.ProjTrans);
    material->UpdateValue("View", RenderManager::pipeline.View);
    material->Bind();
    mesh->Bind();

    std::vector<glm::mat4x3> mats;
    animator->ApplyAnimation(mats, GetMatrix());
    int location = it->second.first;
    glUniformMatrix4x3fv(location, mats.size(), GL_FALSE, glm::value_ptr(mats[0]));

    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }
}

void ComponentSkeletalMesh::SetSkeletalMesh(GeometrySkeletalMesh *new_mesh) {
    if (new_mesh != nullptr) {
        mesh = new_mesh;
        animator = new Animator(mesh->GetSkeleton());
    }
}
