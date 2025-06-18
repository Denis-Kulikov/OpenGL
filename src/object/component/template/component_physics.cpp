#include <object/component/template/component_physics.hpp>


ComponentPhysics::ComponentPhysics(RigidTransform *transform)
    : Component(transform)
{}

void ComponentPhysics::UpdateInverse() {
    invRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    invScale = glm::vec3(1.f);
    invOffset = glm::vec3(0);
}

void ComponentPhysics::UpdateMatrix() {
    localTransform->UpdateMatrix();
}

void ComponentPhysics::UpdateMatrixTree(const glm::mat4x4& parentTR, const glm::mat4x4& parentS) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), GetPosition());
    glm::mat4 R = glm::mat4_cast(GetRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), GetScale());
    auto TR = T * R;
    if (parent) {
        TR = parentTR * TR;
        S = parentS * S;
    }

    for (Component* child : children) {
        child->UpdateMatrixTree(TR, S);
    }
}
