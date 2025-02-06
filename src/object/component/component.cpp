#include <object/component/component.hpp>
#include "glm/gtx/string_cast.hpp"

// Component::Component(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
//     : parent(parent), globalTransform(globalTransform)
// {
//     localTransform = *localTransform_;
//     localTransform->UpdateMatrix();
// }

// Component::Component(Component *parent, Transformable *globalTransform)
//     : parent(parent), globalTransform(globalTransform)
// {
//     localTransform->UpdateMatrix();
// }

Component::Component(Component *parent, Transform *transform)
    : parent(parent), localTransform(transform), globalTransform(new Transform()) {}

Component::Component(Component *parent, RigidTransform *transform)
    : parent(parent), localTransform(transform), globalTransform(transform) {}


Component::~Component() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void Component::UpdateMatrixTree() {
    UpdateMatrix();
    for (Component* child : children) {
        child->UpdateMatrixTree();
    }
}

void Component::UpdateMatrix() {
    if (isMoved) {
        isMoved = false;
        localTransform->UpdateMatrix();
        // if (globalTransform->isMoving()) {
        //     globalTransform->UpdateMatrix();
        //     globalTransform->SetMatrix(localTransform->GetMatrix() * globalTransform->GetMatrix());
        //     *localTransform = Transform();
        //     return;
        // } else {
        // }
    }

    if (!globalTransform->isMoving()) { // RigidTransform - то ничего не происходит
        if (parent)
            globalTransform->SetMatrix(parent->GetMatrix()  * localTransform->GetMatrix());
        else
            globalTransform->SetMatrix(localTransform->GetMatrix());
    } else {
        globalTransform->UpdateMatrix();
    }

    // if (isMoved) {
    //     isMoved = false;
    //     localTransform->UpdateMatrix();

    //     if (globalTransform->isMoving()) {
    //         globalTransform->UpdateMatrix();
    //         globalTransform->SetMatrix(localTransform->GetMatrix() * globalTransform->GetMatrix());
    //         localTransform = Transform();
    //         return;
    //     } else {
            
    //     }
    // }

    // if (globalTransform->isMoving()) {
    //     globalTransform->UpdateMatrix();
    // } else {
    //     if (parent != nullptr) {
    //         globalTransform->SetMatrix(parent->GetMatrix() * localTransform->GetMatrix());
    //     } else {
    //         // globalTransform->SetMatrix(localTransform->GetMatrix());
    //     }
    // }
}

glm::vec3 QuatToEulerDegrees(const glm::quat& quat) {
    glm::vec3 euler = glm::degrees(glm::eulerAngles(quat)); // Получаем углы в радианах и переводим в градусы
    return euler;
}

void quat_to_vec(glm::quat &rotation) {
    glm::vec3 eulerDegrees = QuatToEulerDegrees(rotation);

    std::cout << "Rotation (degrees): X = " << eulerDegrees.x 
              << ", Y = " << eulerDegrees.y 
              << ", Z = " << eulerDegrees.z << std::endl;
}

void Component::Spawn(Transform &startTransform) {
    if (isMoved) {
        isMoved = false;
        localTransform->UpdateMatrix();
    }

    Transform transform = startTransform;

    // glm::vec3 parentScale = GetScale();
    // glm::quat parentRotation = GetRotation();
    // glm::vec3 localPos = glm::inverse(parentRotation) * (startTransform.GetPosition());
    // localPos /= parentScale;
    // glm::quat localRotation = glm::inverse(parentRotation) * startTransform.GetRotation();
    // transform.SetPosition(localPos);
    // transform.SetScale(startTransform.GetScale() / parentScale);
    // transform.SetRotation(localRotation);

    transform.UpdateMatrix();

    localTransform->SetPosition(transform.GetPosition() + localTransform->GetPosition());
    localTransform->SetRotation(transform.GetRotation() * localTransform->GetRotation());
    localTransform->SetScale(transform.GetScale() * localTransform->GetScale());

    // quat_to_vec(localTransform->GetRotation());
    // localTransform->SetMatrix(transform.GetMatrix() * localTransform->GetMatrix());
    // quat_to_vec(localTransform->GetRotation());
    // std::cout << std::endl; 
    isMoved = true;
}

void Component::RenderTree() const {
    Render();
    for (const Component* child : children) {
        child->RenderTree();
    }
}

glm::vec3 Component::GetPosition() const {
    return localTransform->GetPosition();
}

glm::quat Component::GetRotation() const {
    return localTransform->GetRotation();
}

glm::vec3 Component::GetScale() const {
    return localTransform->GetScale();
}

void Component::SetPosition(const glm::vec3& position) {
    isMoved = true;
    if (parent != nullptr) {
        glm::vec3 localPos = glm::inverse(parent->GetRotation()) * (position - parent->GetPosition());
        localPos /= parent->GetScale();
        localTransform->SetPosition(localPos);
    } else {
        localTransform->SetPosition(position);
    }
}
void Component::SetRotation(const glm::quat& rotation) {
    isMoved = true;
    if (parent != nullptr) {
        glm::quat localRotation = glm::inverse(parent->GetRotation()) * rotation;
        localTransform->SetRotation(localRotation);
    } else {
        localTransform->SetRotation(rotation);
    }
}
void Component::SetScale(const glm::vec3& scale) {
        isMoved = true;
    if (parent != nullptr) {
        localTransform->SetScale(scale / parent->GetScale());
    } else {
        localTransform->SetScale(scale);
    }
}

void Component::Move(const glm::vec3& offset) {
    isMoved = true;
    if (parent) {
        glm::vec3 scale = parent->GetScale();
        scale = glm::max(scale, glm::vec3(0.0001f)); // Чтобы избежать деления на 0
        glm::vec3 localOffset = glm::inverse(parent->GetRotation()) * (offset / scale);
        localTransform->SetPosition(localTransform->GetPosition() + localOffset);
    } else {
        localTransform->SetPosition(localTransform->GetPosition() + offset);
    }
}
void Component::Move(glm::vec3 direction, float distance) {
    isMoved = true;
    Move(glm::normalize(direction) * distance);
}
void Component::Rotate(const glm::quat& deltaRotation) {
    isMoved = true;
    if (parent) {
        glm::quat localRotation = glm::inverse(parent->GetRotation()) * deltaRotation;
        localTransform->SetRotation(localTransform->GetRotation() * localRotation);
    } else {
        localTransform->SetRotation(localTransform->GetRotation() * deltaRotation);
    }
}
void Component::RotateAround(const glm::vec3& axis, float angle) {
    isMoved = true;
    glm::quat deltaRotation = glm::angleAxis(angle, glm::normalize(axis));
    Rotate(deltaRotation);
}

// void Component::SetPosition(const glm::vec3& position) {
//     localTransform->SetPosition(position);
// }

// void Component::SetRotation(const glm::quat& rotation) {
//     localTransform->SetRotation(rotation);
// }

// void Component::SetScale(const glm::vec3& scale) {
//     localTransform->SetScale(scale);
// }

// void Component::Move(const glm::vec3& offset) {
//     isMoved = true;
//     localTransform->Move(offset);
// }

// void Component::Move(glm::vec3 direction, float distance) {
//     isMoved = true;
//     localTransform->Move(direction, distance);
// }

// void Component::Rotate(const glm::quat& deltaRotation) {
//     isMoved = true;
//     localTransform->Rotate(deltaRotation);
// }

// void Component::RotateAround(const glm::vec3& axis, float angle) {
//     isMoved = true;
//     localTransform->RotateAround(axis, angle);
// }

void Component::AddChild(Component* child) {
    children.push_back(child);
    child->parent = this;
    
    // child->transform->FixLocalTransform();
    // glm::vec3 parentScale = localTransform->GetScale();
    // glm::quat parentRotation = localTransform->GetRotation();
    // glm::vec3 localPos = glm::inverse(parentRotation) * (child->localTransform->GetPosition());
    // localPos /= parentScale;
    // glm::quat localRotation = glm::inverse(parentRotation) * child->localTransform->GetRotation();
    // child->localTransform->SetPosition(localPos);
    // child->localTransform->SetScale(child->localTransform->GetScale() / parentScale);
    // child->localTransform->SetRotation(localRotation);

    child->localTransform->UpdateMatrix();

    child->SetPosition(child->localTransform->GetPosition());
    child->SetScale(child->localTransform->GetScale());
    child->SetRotation(child->localTransform->GetRotation());
}
