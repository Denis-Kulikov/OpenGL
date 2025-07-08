#include <object/component/component.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

std::string printVec3(const glm::vec3& v);
glm::vec3 quatToEuler(const glm::quat& q);

extern bool fIsInit;


Component::Component(RigidTransform *transform)
    : localTransform(transform), globalTransform(transform)
{
    scale = glm::vec3(1.f);
    invScale = glm::vec3(1.f);
}

Component::Component(TransformableMatrix *transform)
    : localTransform(transform), globalTransform(new Transform()) 
{
    scale = glm::vec3(1.f);
    invScale = glm::vec3(1.f);
}

Component::~Component() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void Component::UpdateInverse() {
    if (parent) {
        invScale = 1.0f / parent->GetLocalScale();
        invScale *= parent->invScale;
    } else {
        invScale = glm::vec3(1.f);
    }
}

void Component::UpdateInverseTree()
{
    UpdateInverse();
    for (Component* child : children) {
        child->UpdateInverseTree();
    }   
}

void Component::UpdateMatrixTree(const glm::mat4x4& parentTR, const glm::mat4x4& parentS) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), GetPosition());
    glm::mat4 R = glm::mat4_cast(GetRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), GetLocalScale());
    auto TR = T * R;
    if (parent) {
        TR = parentTR * TR;
        S = parentS * S;
    }

    globalTransform->SetMatrix(TR * S);

    for (Component* child : children) {
        child->UpdateMatrixTree(TR, S);
    }
}

void Component::RenderTree() const {
    Render();
    for (const Component* child : children) {
        child->RenderTree();
    }
}

void Component::UpdateTree(float deltaTime) {
    Update(deltaTime);
    for (Component* child : children) {
        child->Update(deltaTime);
    }
}

glm::vec3 Component::GetPosition() const {
    return localTransform->GetPosition();
}
glm::quat Component::GetRotation() const {
    return localTransform->GetRotation();
}
glm::vec3 Component::GetScale() const {
    return scale;
}
glm::vec3 Component::GetLocalScale() const {
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

void Component::FixPosition(const glm::vec3& scaleChange) {
    localTransform->SetPosition(GetPosition() * scaleChange);

    for (Component* child : children) {
        child->FixPosition(scaleChange);
    }
}

void Component::SetPosition(const glm::vec3& position) {
    localTransform->SetPosition(position);
}
void Component::SetRotation(const glm::vec3& rotation) {
    glm::quat quaternion = glm::quat(glm::radians(rotation));
    SetRotation(quaternion);
}
void Component::SetRotation(const glm::quat& rotation) {
    localTransform->SetRotation(rotation);
}
void Component::SetScale(const glm::vec3& newScale) {
    scale = newScale;

    if (parent) {
        scale *= invScale;
    }
    
    glm::vec3 scaleChange = scale / GetLocalScale();
    localTransform->SetScale(scale);

    for (auto& child : children) {
        child->FixPosition(scaleChange);
    }
}

void Component::Move(const glm::vec3& offset) {
    localTransform->SetPosition(GetPosition() + offset);
}
void Component::Move(glm::vec3 direction, float distance) {
    if (glm::length(direction) != 0) {
        Move(glm::normalize(direction) * distance);
    }
}
void Component::Rotate(const glm::quat& deltaRotation) {
    localTransform->SetRotation(GetRotation() * deltaRotation);
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
