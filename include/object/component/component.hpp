#pragma once
#include <lib-project/lib.hpp>
#include <object/transform/transformable.hpp>
#include <object/transform/transform.hpp>
#include <btBulletDynamicsCommon.h>


class Component {
public:
    Component(Component *parent, const Transform *localTransform_, Transformable *globalTransform);
    Component(Component *parent, Transformable *globalTransform);
    ~Component();

    void UpdateMatrix();
    void UpdateMatrixTree();
    void Spawn(const Transform *startTransform);

    virtual void Render() const = 0;
    void RenderTree() const;

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    void Move(const glm::vec3& offset);
    void Move(glm::vec3 direction, float distance);
    void Rotate(const glm::quat& deltaRotation);
    void RotateAround(const glm::vec3& axis, float angle);

    Transformable *GetTransform() { return globalTransform; }
    Transform *GetLocalTransform() { return &localTransform; }
    void SetTransform(Transform *new_transformable) { localTransform = *new_transformable; }
    void SetBody(Transformable *new_transformable) { globalTransform = new_transformable; }
    const glm::mat4 &GetMatrix() const { return globalTransform->GetMatrix(); }
    Component *GetParent() const { return parent; }
    std::vector<Component*>& GetChildren() { return children; }
    void AddChild(Component* child) {
        children.push_back(child);
        child->parent = this;
    }

protected:
    void Spawn(const glm::mat4x4 &startTransform);

    Transformable *globalTransform = nullptr;
    Transform localTransform;
    bool isMoved = true;
    Component *parent = nullptr;
    std::vector<Component*> children;
};



template <typename T>
T* CreateComponent(Component *parent, const Transform *localTransform_, Transformable *globalTransform) {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    T* component = new T(parent, localTransform_, globalTransform);
    return component;
}

template <typename T>
T* CreateComponent(Component *parent, Transformable *globalTransform) {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    T* component = new T(parent, globalTransform);
    return component;
}

