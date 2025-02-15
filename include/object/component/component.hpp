#pragma once
#include <object/transform/transform.hpp>


class Component {
public:
    // Component(Component *parent, const Transform *localTransform_, Transformable *globalTransform);
    Component(Transform *transform);
    // Component(RigidTransform *transform);
    ~Component();

    void UpdateInverseTransform();
    void UpdateInverseTransformTree();
    void UpdateMatrix();
    void UpdateMatrixTree();
    void Spawn(Transform &startTransform);

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
    Transformable *GetLocalTransform() { return localTransform; }
    void SetTransform(Transform &new_transformable) { *localTransform = new_transformable; }
    const glm::mat4 &GetMatrix() const { return globalTransform->GetMatrix(); }
    Component *GetParent() const { return parent; }
    std::vector<Component*>& GetChildren() { return children; }
    void AddChild(Component* child);

// protected:
    // void Spawn(const glm::mat4x4 &startTransform);

    std::vector<Component*> children;
    Component *parent = nullptr;
    Transformable *globalTransform = nullptr;
    Transformable *localTransform = nullptr;
    glm::mat4x4 inverseTransform;
    bool isMoved = false;
};



// template <typename T>
// T* CreateComponent(Component *parent, Transform *transform) {
//     static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
//     T* component = new T(parent, transform);
//     return component;
// }

// template <typename T>
// T* CreateComponent(Component *parent, RigidTransform *transform) {
//     static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
//     T* component = new T(parent, transform);
//     return component;
// }

template <typename C>
C* CreateComponent() {
    static_assert(std::is_base_of_v<Component, C>, "T must derive from Component");

    return new C(new Transform());
}

template <typename C, typename T>
C* CreateComponent(T *transform) {
    static_assert(std::is_base_of_v<Component, C>, "T must derive from Component");
    static_assert(std::is_base_of_v<Transformable, T>, "TransformType must derive from Transformable");

    return new C(transform);
}