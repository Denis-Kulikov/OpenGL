#pragma once
#include <btBulletDynamicsCommon.h>
#include <lib-project/lib.hpp>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
        : Component(parent, localTransform_, globalTransform) {}
    ComponentMesh(Component *parent, Transformable *globalTransform)
        : Component(parent, globalTransform) {}

    void Render() const override {
        mesh->set_transform(GetMatrix());
        objectTransform t;
        t.SetWorldPos(globalTransform->GetPosition());
        glm::vec3 eulerAngles = glm::eulerAngles(globalTransform->GetRotation());
        t.SetRotate(glm::degrees(eulerAngles)); // Преобразование в градусы, если требуется
        // t.SetRotate(transform->GetRotation());
        t.SetScale(globalTransform->GetScale());
        mesh->set_transform(t);
        std::vector<aiMatrix4x4> Transforms;
        mesh->BoneTransform(GameManager::Time.GetCurrentTime(), Transforms);
        mesh->Render(&Transforms);
    }

// private:
    Mesh *mesh;
};
