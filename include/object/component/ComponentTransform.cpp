class ComponentTransform
{
public:
    ComponentTransform(ComponentTransform *parent, Transform *transform)
        : parent(parent), transform(transform), globalTransform(new Transform()), Moving(false) {}
    ComponentTransform(ComponentTransform *parent, RigidTransform *transform)
        : parent(parent), transform(transform), globalTransform(transform), Moving(true) {}
    ~ComponentTransform() {
        if (globalTransform && globalTransform != transform) { // Проверяем, не указывает ли globalTransform на transform
            delete globalTransform;
        }
        delete transform;
    }

    void SetParent(ComponentTransform *parent_) {
        parent = parent_;
    }
    
    bool isMoving() { return Moving; }

    void UpdateMatrix() { 
        if (isMoved) {
            isMoved = false;
            transform->UpdateMatrix(); 
        }
    }
    void UpdateGlobalMatrix() { 
        // Будут баги если RigidTransform - дочерний компонент
        if (!isMoving()) { // RigidTransform - то ничего не происходит
            if (parent)
                globalTransform->SetMatrix(parent->GetGlobalTransform()->GetMatrix() * transform->GetMatrix());
            else
                globalTransform->SetMatrix(transform->GetMatrix());
        } else {
            globalTransform->UpdateMatrix();
        }
    }

    void Spawn(const Transform &startTransform) {
        globalTransform->SetMatrix(startTransform.GetMatrix() * transform->GetMatrix());
    };
    
    const Transformable *GetGlobalTransform() const { return globalTransform; }

    glm::vec3 GetPosition() const { return transform->GetPosition(); };
    glm::quat GetRotation() const { return transform->GetRotation(); };
    glm::vec3 GetScale()    const { return transform->GetScale(); };

    void SetPosition(const glm::vec3& position) {
        isMoved = true;
        if (parent != nullptr) {
            glm::vec3 localPos = glm::inverse(parent->GetRotation()) * (position - parent->GetPosition());
            localPos /= parent->GetScale();
            transform->SetPosition(localPos);
        } else {
            transform->SetPosition(position);
        }
    }
    void SetRotation(const glm::quat& rotation) {
        isMoved = true;
        if (parent != nullptr) {
            glm::quat localRotation = glm::inverse(parent->GetRotation()) * rotation;
            transform->SetRotation(localRotation);
        } else {
            transform->SetRotation(rotation);
        }
    }
    void SetScale(const glm::vec3& scale) {
            isMoved = true;
        if (parent != nullptr) {
            transform->SetScale(scale / parent->GetScale());
        } else {
            transform->SetScale(scale);
        }
    }

    void Move(const glm::vec3& offset) {
        isMoved = true;
        if (parent) {
            glm::vec3 scale = parent->GetScale();
            scale = glm::max(scale, glm::vec3(0.0001f)); // Чтобы избежать деления на 0
            glm::vec3 localOffset = glm::inverse(parent->GetRotation()) * (offset / scale);
            transform->SetPosition(transform->GetPosition() + localOffset);
        } else {
            transform->SetPosition(transform->GetPosition() + offset);
        }
    }
    void Move(glm::vec3 direction, float distance) {
        isMoved = true;
        Move(glm::normalize(direction) * distance);
    }
    void Rotate(const glm::quat& deltaRotation) {
        isMoved = true;
        if (parent) {
            glm::quat localRotation = glm::inverse(parent->GetRotation()) * deltaRotation;
            transform->SetRotation(transform->GetRotation() * localRotation);
        } else {
            transform->SetRotation(transform->GetRotation() * deltaRotation);
        }
    }
    void RotateAround(const glm::vec3& axis, float angle) {
        isMoved = true;
        glm::quat deltaRotation = glm::angleAxis(angle, glm::normalize(axis));
        Rotate(deltaRotation);
    }

    void FixLocalTransform() {
        SetPosition(transform->GetPosition());
        SetRotation(transform->GetRotation());
        SetScale(transform->GetScale());
        // glm::vec3 parentScale = parent->GetScale();
        // glm::quat parentRotation = parent->GetRotation();

        // glm::vec3 localPos = glm::inverse(parentRotation) * (transform->GetPosition());
        // localPos /= parentScale;
        // glm::quat localRotation = glm::inverse(parentRotation) * transform->GetRotation();

        // transform->SetPosition(localPos);
        // transform->SetScale(transform->GetScale() / parentScale);
        // transform->SetRotation(localRotation);
    }

protected:
    bool isMoved = false;
    bool Moving = false;
    ComponentTransform *parent;
    Transformable *globalTransform;
    Transformable *transform;
};

#include <object/component/component.hpp>
#pragma once
#include <lib-project/lib.hpp>
#include <object/transform/transformable.hpp>
#include <object/transform/transform.hpp>
#include <btBulletDynamicsCommon.h>
#include "component_transform.hpp"


class Component {
public:
    Component(Component *parent, Transform *transform);
    Component(Component *parent, RigidTransform *transform);
    Component(std::nullptr_t, Transform *transform);
    Component(std::nullptr_t, RigidTransform *transform);
    ~Component();

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

    ComponentTransform *GetTransform() { return &transform; }

    const glm::mat4 &GetMatrix() const { return transform.GetGlobalTransform()->GetMatrix(); }
    Component *GetParent() const { return parent; }
    std::vector<Component*>& GetChildren() { return children; }
    void AddChild(Component* child);

// protected:
    ComponentTransform transform;
    bool isMoved = false;
    Component *parent = nullptr;
    std::vector<Component*> children;
};


    // Component(Component *parent, Transform *transform);
    // Component(Component *parent, RigidTransform *transform);
    // Component(std::nullptr_t, Transform *transform);
    // Component(std::nullptr_t, RigidTransform *transform);

template <typename T, typename TransformType>
std::enable_if_t<std::is_base_of_v<Component, T> && 
                 std::is_base_of_v<Transformable, TransformType>, T*>
CreateComponent(Component* parent, TransformType* transform) {
    return new T(parent, transform);
}

template <typename T, typename TransformType>
std::enable_if_t<std::is_base_of_v<Component, T> && 
                 std::is_base_of_v<Transformable, TransformType>, T*>
CreateComponent(std::nullptr_t, TransformType* transform) {
    return new T(nullptr, transform);
}

Component::Component(Component *parent, Transform *transform)
    : parent(parent), transform(&parent->transform, transform) {}
Component::Component(Component *parent, RigidTransform *transform)
    : parent(parent), transform(&parent->transform, transform) {}

Component::Component(std::nullptr_t, Transform *transform)
    : parent(nullptr), transform(nullptr, transform) {}
Component::Component(std::nullptr_t, RigidTransform *transform)
    : parent(nullptr), transform(nullptr, transform) {}

Component::~Component() {
}

void Component::UpdateMatrixTree() {
    UpdateMatrix();
    for (Component* child : children) {
        child->UpdateMatrixTree();
    }
}

// void Component::UpdateMatrix() {
//     if (isMoved) {
//         isMoved = false;
//         localTransform.UpdateMatrix();

//         if (globalTransform->isMoving()) {
//             globalTransform->UpdateMatrix();
//             globalTransform->SetMatrix(localTransform.GetMatrix() * globalTransform->GetMatrix());
//             localTransform = Transform();
//             return;
//         } else {
            
//         }
//     }

//     if (globalTransform->isMoving()) {
//         globalTransform->UpdateMatrix();
//     } else {
//         if (parent != nullptr) {
//             globalTransform->SetMatrix(parent->GetMatrix() * localTransform.GetMatrix());
//         } else {
//             // globalTransform->SetMatrix(localTransform.GetMatrix());
//         }
//     }
// }

void Component::UpdateMatrix() {
    transform.UpdateMatrix();
    transform.UpdateGlobalMatrix();
}

// void Component::Spawn(const Transform *startTransform) {
//     isMoved = true;
//     Transform transform;
//     transform.SetPosition(startTransform->GetPosition() + localTransform.GetPosition());
//     transform.SetRotation(startTransform->GetRotation() * localTransform.GetRotation());
//     transform.SetScale(startTransform->GetScale() * localTransform.GetScale());
//     transform.UpdateMatrix();

//     globalTransform->SetMatrix(transform.GetMatrix() * localTransform.GetMatrix());
//     globalTransform->UpdateTransform();

//     if (globalTransform->isMoving()) {
//         localTransform = Transform();
//     }

//     for (Component* child : children) {
//         child->Spawn(globalTransform->GetMatrix());
//     }
// }

void Component::Spawn(Transform &startTransform) {
    transform.UpdateMatrix();
    startTransform.UpdateMatrix();
    transform.Spawn(startTransform);

    // if (globalTransform->isMoving()) {
    //     localTransform = Transform();
    // }

    for (Component* child : children) {
        // child->Spawn(globalTransform->GetMatrix());
        child->UpdateMatrixTree();
    }
}

void Component::RenderTree() const {
    Render();
    for (const Component* child : children) {
        child->RenderTree();
    }
}

glm::vec3 Component::GetPosition() const {
    return transform.GetPosition();
}

glm::quat Component::GetRotation() const {
    return transform.GetRotation();
}

glm::vec3 Component::GetScale() const {
    return transform.GetScale();
}

void Component::SetPosition(const glm::vec3& position) {
    transform.SetPosition(position);
}

void Component::SetRotation(const glm::quat& rotation) {
    transform.SetRotation(rotation);
}

void Component::SetScale(const glm::vec3& scale) {
    transform.SetScale(scale);
}

void Component::Move(const glm::vec3& offset) {
    isMoved = true;
    transform.Move(offset);
}

void Component::Move(glm::vec3 direction, float distance) {
    isMoved = true;
    transform.Move(direction, distance);
}

void Component::Rotate(const glm::quat& deltaRotation) {
    isMoved = true;
    transform.Rotate(deltaRotation);
}

void Component::RotateAround(const glm::vec3& axis, float angle) {
    isMoved = true;
    transform.RotateAround(axis, angle);
}

void Component::AddChild(Component* child) {
    children.push_back(child);
    child->parent = this;
    child->transform.SetParent(&this->transform);
    child->transform.FixLocalTransform();

    // glm::vec3 parentScale = localTransform.GetScale();
    // glm::quat parentRotation = localTransform.GetRotation();

    // glm::vec3 localPos = glm::inverse(parentRotation) * (child->localTransform.GetPosition());
    // localPos /= parentScale;
    // glm::quat localRotation = glm::inverse(parentRotation) * child->localTransform.GetRotation();

    // child->localTransform.SetPosition(localPos);
    // child->localTransform.SetScale(child->localTransform.GetScale() / parentScale);
    // child->localTransform.SetRotation(localRotation);
}
