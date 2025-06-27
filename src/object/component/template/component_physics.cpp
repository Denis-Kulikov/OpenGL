#include <object/component/template/component_physics.hpp>


ComponentPhysics::ComponentPhysics(RigidTransform *transform)
    : Component(transform)
{}

// void ComponentPhysics::UpdateMatrix() {
//     localTransform->UpdateMatrix();
// }

// void ComponentPhysics::UpdateMatrixTree(const glm::mat4x4& parentTR = glm::mat4(1.0f), const glm::mat4x4& parentS = glm::mat4(1.0f)) {
//     glm::mat4 T = glm::translate(glm::mat4(1.0f), GetPosition());
//     glm::mat4 R = glm::mat4_cast(GetRotation());
//     glm::mat4 S = glm::scale(glm::mat4(1.0f), GetLocalScale());
//     auto TR = T * R;
//     if (parent) {
//         TR = parentTR * TR;
//         S = parentS * S;
//     }

//     *globalTransform = TR * S;

//     for (Component* child : children) {
//         child->UpdateMatrixTree(TR, S);
//     }
// }

