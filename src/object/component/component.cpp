#include <object/component/component.hpp>

Component::Component(RigidTransform *transform)
    : localTransform(transform), globalTransform(transform) 
{}

Component::Component(Transform *transform)
    : localTransform(transform), globalTransform(new Transform()) 
{}

Component::~Component() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void Component::UpdateInverse() { // virtual
    if (parent) {
        parent->localTransform->UpdateMatrix();
        inverseTransform = glm::inverse(parent->localTransform->GetMatrix()) * parent->inverseTransform.GetMatrix();
    } else
        inverseTransform = glm::mat4x4(1.0f);
}

void Component::UpdateInverseTree()
{
    UpdateInverse();
    for (Component* child : children) {
        child->UpdateInverseTree();
    }   
}

void Component::UpdateMatrixTree() {
    UpdateMatrix();
    for (Component* child : children) {
        child->UpdateMatrixTree();
    }
}

void Component::UpdateMatrix() { // virtual
    localTransform->UpdateMatrix();
    *globalTransform = parent ? parent->GetMatrix() * localTransform->GetMatrix() : localTransform->GetMatrix();
}

void Component::Spawn(const Transform &startTransform) {
    localTransform->UpdateMatrix();

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

glm::vec3 Component::GetGlobalPosition() const {
    return globalTransform->GetPosition();
}

glm::quat Component::GetGlobalRotation() const {
    return globalTransform->GetRotation();
}

glm::vec3 Component::GetGlobalScale() const {
    return globalTransform->GetScale();
}

void Component::SetPosition(const glm::vec3& position) {
    if (parent) {
        glm::vec4 worldPos = glm::vec4(position, 1.0f);
        glm::vec4 localPos = inverseTransform.GetMatrix() * worldPos;
        localTransform->SetPosition(glm::vec3(localPos));
    } else {
        localTransform->SetPosition(position);
    }
}
void Component::SetRotation(const glm::vec3& rotation) {
    glm::quat quaternion = glm::quat(glm::radians(rotation));
    SetRotation(quaternion);
}
void Component::SetRotation(const glm::quat& rotation) {
    localTransform->SetRotation(rotation);
}
void Component::SetScale(const glm::vec3& scale) {
    if (parent) {
        glm::vec3 parentScale = inverseTransform.GetScale();
        localTransform->SetScale(scale * parentScale);
    } else {
        localTransform->SetScale(scale);
    }
}

void Component::Move(const glm::vec3& offset) {
    localTransform->SetPosition(localTransform->GetPosition() + offset);
}
void Component::Move(glm::vec3 direction, float distance) {
    if (glm::length(direction) != 0) {
        Move(glm::normalize(direction) * distance);
    }
}
void Component::Rotate(const glm::quat& deltaRotation) {
    localTransform->SetRotation(localTransform->GetRotation() * deltaRotation);
}
void Component::RotateAround(const glm::vec3& axis, float angle) {
    glm::quat deltaRotation = glm::angleAxis(angle, glm::normalize(axis));
    Rotate(deltaRotation);
}

void Component::AddChild(Component* child) {
    children.push_back(child);
    child->parent = this;

    UpdateInverse();
    child->UpdateInverse();
    child->localTransform->UpdateMatrix();
    *child->localTransform = child->inverseTransform.GetMatrix() * child->localTransform->GetMatrix();
}
