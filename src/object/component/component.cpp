#include <object/component/component.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

std::string printVec3(const glm::vec3& v);
glm::vec3 quatToEuler(const glm::quat& q);


Component::Component(RigidTransform *transform)
    : localTransform(transform), globalTransform(transform)
{
    invRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    invScale = glm::vec3(1.f);
    invOffset = glm::vec3(0);
}

Component::Component(Transform *transform)
    : localTransform(transform), globalTransform(new Transform()) 
{
    invRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    invScale = glm::vec3(1.f);
    invOffset = glm::vec3(0);
}

Component::~Component() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void Component::UpdateInverse() {
    if (parent) {
        Transform invScaleMat4x4;
        invScaleMat4x4 = glm::inverse(glm::scale(glm::mat4(1.0f), parent->GetScale()));
        invScaleMat4x4.UpdateTransform();
        invScale = parent->invScale * invScaleMat4x4.GetScale();

        invRot = parent->invRot * glm::inverse(parent->GetRotation());

        invOffset = parent->invOffset + parent->GetPosition();
        // invOffset = parent->invOffset + parent->GetPosition();

        // std::cout << "invScale: " << printVec3(invScale) << std::endl;
        // std::cout << "invRot: " << printVec3(quatToEuler(invRot)) << std::endl;
        // std::cout << "parent->GetPosition: " << printVec3(parent->GetPosition()) << std::endl;
        // std::cout << "invOffset: " << printVec3(invOffset) << std::endl;
        // std::cout << std::endl;
    } else {
        invRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        invScale = glm::vec3(1.f);
        invOffset = glm::vec3(0);
    }
}

void Component::UpdateInverseTree()
{
    UpdateInverse();
    for (Component* child : children) {
        child->UpdateInverseTree();
    }   
}

// void Component::UpdateMatrixTree() {
//     UpdateMatrix();
//     for (Component* child : children) {
//         child->UpdateMatrixTree();
//     }
// }

void Component::UpdateMatrixTree(const glm::mat4x4& parentTR, const glm::mat4x4& parentS) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), GetPosition());
    glm::mat4 R = glm::mat4_cast(GetRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), GetScale());
    auto TR = T * R;
    if (parent) {
        TR = parentTR * TR;
        S = parentS * S;
    }

    *globalTransform = TR * S;

    for (Component* child : children) {
        child->UpdateMatrixTree(TR, S);
    }
}

void Component::UpdateMatrix() {
    localTransform->UpdateMatrix();
    *globalTransform = parent ? parent->GetMatrix() * localTransform->GetMatrix(): 
                                localTransform->GetMatrix();
    globalTransform->UpdateTransform();
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

void Component::FixPosition(const glm::vec3& scale) {
    localTransform->SetPosition(GetPosition() * scale);

    for (Component* child : children) {
        child->FixPosition(scale);
    }
}

void Component::SetPosition(const glm::vec3& position) {
    if (parent) {
        auto local = glm::inverse(parent->GetRotation()) * (position - invOffset);
        localTransform->SetPosition(local);
    } else {
        localTransform->SetPosition(position);
    }
}
void Component::SetRotation(const glm::vec3& rotation) {
    glm::quat quaternion = glm::quat(glm::radians(rotation));
    SetRotation(quaternion);
}
void Component::SetRotation(const glm::quat& rotation) {
    if (parent) {
        localTransform->SetRotation(invRot * rotation);
    } else {
        localTransform->SetRotation(rotation);
    }
}
void Component::SetScale(glm::vec3 scale) {
    if (parent) {
        scale *= invScale;
    }
    
    glm::vec3 scaleChange = scale / localTransform->GetScale();
    localTransform->SetScale(scale);

    for (auto& child : children) {
        child->FixPosition(scale);
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

    child->SetPosition({0, 0, 0});
    child->SetRotation({0, 0, 0});
    child->SetScale({1, 1, 1});
}
