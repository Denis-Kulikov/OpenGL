#include <managers/render_manager.hpp>
#include <object/component/template/skeletal_mesh.hpp>

void ComponentSkeletalMesh::UpdateInverse() {
    localTransform->UpdateMatrix();
    if (parent)
        inverseTransform = glm::inverse(parent->localTransform->GetMatrix()) * parent->inverseTransform.GetMatrix();
    else
        inverseTransform = glm::mat4x4(1.0f);

    for (auto& children : attachedToBone) {
        // В следующей строке вычисление children.second->inverseTransform не правильно не учитывается обратная матрица кости
        // У кости обратнрая матрица уже учитывает иерархическую структуру, нужно просто обратиться к нужной кости
        // Суть проблемы в том что я не знаю в каком порядке умножить матрицы
        // Нужно добавить mesh->skeleton.BoneInvers[children.first];
        // children.second->inverseTransform = mesh->skeleton.BoneInvers[children.first] *
        children.second->inverseTransform = 
                                            glm::inverse(localTransform->GetMatrix() * inverseTransform.GetMatrix());
        for (auto& c : children.second->children) {
            children.second->UpdateInverseTree();
        }
    }
}
void ComponentSkeletalMesh::UpdateMatrix() {
    localTransform->UpdateMatrix();
    *globalTransform = parent ? parent->GetMatrix() * localTransform->GetMatrix() : localTransform->GetMatrix();

    animator->Update(mats);

    for (auto& children : attachedToBone) {
        children.second->localTransform->UpdateMatrix();
        // *children.second->globalTransform = mats[children.first] * children.second->localTransform->GetMatrix();
        *children.second->globalTransform = globalTransform->GetMatrix() * children.second->localTransform->GetMatrix();
        for (auto& c : children.second->children) {
            c->UpdateMatrixTree();
        }
    }
}

void ComponentSkeletalMesh::Render() const {
    const auto it = material->values.find("gBones");
    if (it == material->values.end()) {
        return;
    }

    material->UpdateValue("Projection", RenderManager::render.ProjTrans);
    material->UpdateValue("View", RenderManager::render.View);
    material->Bind();
    mesh->Bind();

    int location = it->second.first;
    glUniformMatrix4fv(location, mats.size(), GL_FALSE, glm::value_ptr(mats[0]));

    for (int i = 0; i < mesh->size(); ++i) {
        if (!material->GetTexture().empty()) {
            int index = mesh->GetTextureIndex(i);
            material->GetTexture()[index]->Bind();
        }
        mesh->Draw(&i);
    }

    for (auto& children : attachedToBone) {
        children.second->RenderTree();
    }
}

void ComponentSkeletalMesh::SetSkeletalMesh(GeometrySkeletalMesh *new_mesh) {
    if (new_mesh != nullptr) {
        mesh = new_mesh;
        animator = new Animator(*this, mesh->skeleton);
    }
}

void ComponentSkeletalMesh::AttachToBone(int boneIndex, Component* component) {
    attachedToBone.push_back({boneIndex, component});
    auto child = attachedToBone.back().second;
    child->parent = this;

    UpdateInverse();
    UpdateMatrix();
    *child->localTransform = child->inverseTransform.GetMatrix() * child->localTransform->GetMatrix();
}
