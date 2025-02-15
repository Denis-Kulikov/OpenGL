#include <object/component/component.hpp>
#include "glm/gtx/string_cast.hpp"


Component::Component(Transform *transform)
    : localTransform(transform), globalTransform(new Transform()) {
    UpdateInverseTransform();
}

// Component::Component(RigidTransform *transform)
//     : localTransform(transform), globalTransform(transform) {
//     UpdateInverseTransform();
// }


Component::~Component() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void Component::UpdateInverseTransform() {
    localTransform->UpdateMatrix();
    inverseTransform = glm::inverse(localTransform->GetMatrix()); 
    if (parent) {
        inverseTransform = inverseTransform * parent->inverseTransform;
    }
}

void Component::UpdateInverseTransformTree()
{
    UpdateInverseTransform();
    for (Component* child : children) {
        child->UpdateInverseTransformTree();
    }   
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
    }

    if (!globalTransform->isMoving()) { // RigidTransform - то ничего не происходит
        if (parent)
            globalTransform->SetMatrix(parent->GetMatrix()  * localTransform->GetMatrix());
        else
            globalTransform->SetMatrix(localTransform->GetMatrix());
    } else {
        globalTransform->UpdateMatrix();
    }
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

    transform.UpdateMatrix();

    localTransform->SetPosition(transform.GetPosition() + localTransform->GetPosition());
    localTransform->SetRotation(transform.GetRotation() * localTransform->GetRotation());
    localTransform->SetScale(transform.GetScale() * localTransform->GetScale());
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
    localTransform->SetPosition(position);
}
void Component::SetRotation(const glm::quat& rotation) {
    localTransform->SetRotation(rotation);
}
void Component::SetScale(const glm::vec3& scale) {
    isMoved = true;
    localTransform->SetScale(scale);
}

void Component::Move(const glm::vec3& offset) {
    isMoved = true;
    localTransform->SetPosition(localTransform->GetPosition() + offset);
}
void Component::Move(glm::vec3 direction, float distance) {
    isMoved = true;
    Move(glm::normalize(direction) * distance);
}
void Component::Rotate(const glm::quat& deltaRotation) {
    isMoved = true;
    localTransform->SetRotation(localTransform->GetRotation() * deltaRotation);
}
void Component::RotateAround(const glm::vec3& axis, float angle) {
    isMoved = true;
    glm::quat deltaRotation = glm::angleAxis(angle, glm::normalize(axis));
    Rotate(deltaRotation);
}

void Component::AddChild(Component* child) {
    children.push_back(child);
    child->parent = this;
    
    child->localTransform->UpdateMatrix();
    child->localTransform->SetMatrix(inverseTransform * child->localTransform->GetMatrix());
    child->UpdateInverseTransform();
}
