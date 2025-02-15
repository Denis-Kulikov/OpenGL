#pragma once
#include <lib-project/lib.hpp>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include "../component.hpp"

class ComponentMesh : public Component {
public:
    ComponentMesh(Transform *transform)
        : Component(transform) {}
    // ComponentMesh(RigidTransform *transform)
    //     : Component(transform) {}

    // ComponentMesh(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
    //     : Component(parent, localTransform_, globalTransform) {}
    // ComponentMesh(Component *parent, Transformable *globalTransform)
    //     : Component(parent, globalTransform) {}

    void Render() const override {
        glm::mat4x4 t = glm::transpose(GetMatrix());
        mesh->set_transform(t);
        std::vector<aiMatrix4x4> Transforms;
        mesh->BoneTransform(GameManager::Time.GetCurrentTime(), Transforms);
        mesh->Render(&Transforms);
    }

// private:
    Mesh *mesh;
};
