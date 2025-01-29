#include <object/component/component.hpp>
#include <game/gameManager.hpp>

Component::Component(Component *parent, const Transform *localTransform_, Transformable *globalTransform)
    : parent(parent), globalTransform(globalTransform)
{
    localTransform = *localTransform_;
    localTransform.UpdateMatrix();
}

Component::Component(Component *parent, Transformable *globalTransform)
    : parent(parent), globalTransform(globalTransform)
{
    localTransform.UpdateMatrix();
}

Component::~Component() { delete globalTransform; }

void Component::UpdateMatrixTree() {
    UpdateMatrix();
    for (Component* child : children) {
        child->UpdateMatrixTree();
    }
}

void Component::UpdateMatrix() {
    if (isMoved) {
        localTransform.UpdateMatrix();
        isMoved = false;

        if (globalTransform->isMoving()) {
            globalTransform->UpdateMatrix();
            globalTransform->SetMatrix(localTransform.GetMatrix() * globalTransform->GetMatrix());
            localTransform = Transform();
            return;
        }
    }

    if (globalTransform->isMoving()) {
        globalTransform->UpdateMatrix();
    } else {
        if (parent != nullptr) {
            globalTransform->SetMatrix(localTransform.GetMatrix() * parent->GetMatrix());
        } else {
            globalTransform->SetMatrix(localTransform.GetMatrix());
        }
    }
}

void Component::Spawn(const Transform *startTransform) {
    isMoved = true;
    Transform transform;
    transform.SetPosition(localTransform.GetPosition() + startTransform->GetPosition());
    transform.SetRotation(localTransform.GetRotation() * startTransform->GetRotation());
    transform.SetScale(localTransform.GetScale() * startTransform->GetScale());
    transform.UpdateMatrix();

    globalTransform->SetMatrix(localTransform.GetMatrix() * transform.GetMatrix());

    if (globalTransform->isMoving()) {
        localTransform = Transform();
    }

    for (Component* child : children) {
        child->Spawn(globalTransform->GetMatrix());
    }
}

void Component::Spawn(const glm::mat4x4 &startTransform)
{
    isMoved = true;
    globalTransform->SetMatrix(localTransform.GetMatrix() * startTransform);

    if (globalTransform->isMoving()) {
        localTransform = Transform();
    }

    for (Component* child : children) {
        child->Spawn(globalTransform->GetMatrix());
    }
}

void Component::RenderTree() const {
    Render();
    for (const Component* child : children) {
        child->RenderTree();
    }
}

glm::vec3 Component::GetPosition() const {
    return localTransform.GetPosition();
}

glm::quat Component::GetRotation() const {
    return localTransform.GetRotation();
}

glm::vec3 Component::GetScale() const {
    return localTransform.GetScale();
}

void Component::SetPosition(const glm::vec3& position) {
    localTransform.SetPosition(position);
}

void Component::SetRotation(const glm::quat& rotation) {
    localTransform.SetRotation(rotation);
}

void Component::SetScale(const glm::vec3& scale) {
    localTransform.SetScale(scale);
}

void Component::Move(const glm::vec3& offset) {
    isMoved = true;
    localTransform.Move(offset);
}

void Component::Move(glm::vec3 direction, float distance) {
    isMoved = true;
    localTransform.Move(direction, distance);
}

void Component::Rotate(const glm::quat& deltaRotation) {
    isMoved = true;
    localTransform.Rotate(deltaRotation);
}

void Component::RotateAround(const glm::vec3& axis, float angle) {
    isMoved = true;
    localTransform.RotateAround(axis, angle);
}

