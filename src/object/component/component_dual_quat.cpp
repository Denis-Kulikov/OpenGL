#include <object/component/component_dual_quat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

std::string printVec3(const glm::vec3& v);
glm::vec3 quatToEuler(const glm::quat& q);

extern bool fIsInit;


ComponentDualQuat::ComponentDualQuat(TransformDualQuat *transform)
    : localTransform(transform), globalTransform(new TransformDualQuat()) 
{}

ComponentDualQuat::~ComponentDualQuat() {
    if (globalTransform != localTransform)
        delete globalTransform;
    delete localTransform;
}

void ComponentDualQuat::UpdateQuats(TransformDualQuat* parentTransform) {
    // TransformDualQuat DQ;

    // glm::vec3 globalTransVec3 = parentTransform->GetPosition() + parentTransform->GetRotation() * GetPosition();
    // DQ.SetRotation(parentTransform->GetRotation() * GetRotation());
    // DQ.SetPosition(glm::quat(0, globalTransVec3.x, globalTransVec3.y, globalTransVec3.z));

    // glm::quat dq.real = DQ.GetRotation();
    // glm::quat dq.dual = 0.5f * (DQ.GetPosition() * DQ.GetRotation());

    // globalTransform->SetRotation(dq.real);
    // globalTransform->SetPosition(dq.dual);

    if (!parentTransform) {
        globalTransform->dq = localTransform->dq;
    } else {
        globalTransform->dq = parentTransform->dq * localTransform->dq;
    }

    for (ComponentDualQuat* child : children) {
        child->UpdateQuats(globalTransform);
    }
}

void ComponentDualQuat::RenderTree() const {
    Render();
    for (const ComponentDualQuat* child : children) {
        child->RenderTree();
    }
}

void ComponentDualQuat::UpdateTree(float deltaTime) {
    Update(deltaTime);
    for (ComponentDualQuat* child : children) {
        child->Update(deltaTime);
    }
}

glm::vec3 ComponentDualQuat::GetPosition() const {
    return localTransform->GetPosition();
}
glm::quat ComponentDualQuat::GetRotation() const {
    return localTransform->GetRotation();
}
glm::vec3 ComponentDualQuat::GetScale() const {
    return scale;
}
glm::vec3 ComponentDualQuat::GetLocalScale() const {
    return localTransform->GetScale();
}

glm::vec3 ComponentDualQuat::GetGlobalPosition() const {
    return globalTransform->GetPosition();
}
glm::quat ComponentDualQuat::GetGlobalRotation() const {
    return globalTransform->GetRotation();
}
glm::vec3 ComponentDualQuat::GetGlobalScale() const {
    return globalTransform->GetScale();
}


void ComponentDualQuat::SetPosition(const glm::vec3& position) {
    localTransform->SetPosition(position);
}
void ComponentDualQuat::SetRotation(const glm::vec3& rotation) {
    glm::quat quaternion = glm::quat(glm::radians(rotation));
    SetRotation(quaternion);
}
void ComponentDualQuat::SetRotation(const glm::quat& rotation) {
    localTransform->SetRotation(rotation);
}
void ComponentDualQuat::SetScale(const glm::vec3& newScale) {}



void ComponentDualQuat::AddChild(ComponentDualQuat* child) {
    children.push_back(child);
    child->parent = this;
}
