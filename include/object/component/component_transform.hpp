#pragma once
#include <object/transform/transformable.hpp>
#include <object/transform/transform.hpp>
#include <object/transform/rigid_transform.hpp>
#include <btBulletDynamicsCommon.h>

class Component;

// class ComponentTransformBase {
// public:
//     ComponentTransformBase(ComponentTransformBase *parent)
//         : parent(parent)
//     {}

//     virtual void UpdateMatrix();
//     virtual void Spawn(const Transform *startTransform);

//     virtual glm::vec3 GetPosition() const;
//     virtual glm::quat GetRotation() const;
//     virtual glm::vec3 GetScale() const;

//     virtual void SetPosition(const glm::vec3& position);
//     virtual void SetRotation(const glm::quat& rotation);
//     virtual void SetScale(const glm::vec3& scale);

//     virtual void Move(const glm::vec3& offset);
//     virtual void Move(glm::vec3 direction, float distance);
//     virtual void Rotate(const glm::quat& deltaRotation);
//     virtual void RotateAround(const glm::vec3& axis, float angle);

//     virtual void FixLocalTransform();
//     // {
//     //     glm::vec3 parentScale = parent->GetLocalScale();
//     //     glm::quat parentRotation = parent->GetLocalRotation();
//     //     glm::vec3 localPos = glm::inverse(parentRotation) * (Realtransform->GetPosition());
//     //     localPos /= parentScale;
//     //     glm::quat localRotation = glm::inverse(parentRotation) * Realtransform->GetRotation();
//     //     localtransform->SetPosition(localPos);
//     //     localtransform->SetScale(Realtransform->GetScale() / parentScale);
//     //     localtransform->SetRotation(localRotation);
//     // }

// protected:
//     bool isMoved = false;
//     ComponentTransformBase *parent;
// };

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
