#include <object/transform/transform_dual_quat.hpp>


TransformDualQuat::TransformDualQuat() : dq({glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)}) {}

glm::quat TransformDualQuat::GetPositionQuat() const {
    return dq.dual;
}
glm::vec3 TransformDualQuat::GetPosition() const {
    glm::quat t = 2.0f * dq.dual * glm::conjugate(dq.real);
    return glm::vec3(t.x, t.y, t.z);
}
glm::quat TransformDualQuat::GetRotation() const { return dq.real; }
glm::vec3 TransformDualQuat::GetScale()    const { return glm::vec3(1.f); }

void TransformDualQuat::SetPosition(const glm::quat& position) { moved(); dq.dual = position; }
void TransformDualQuat::SetPosition(const glm::vec3& position) {
    moved();
    glm::quat r = dq.real;
    glm::quat t(0, position.x, position.y, position.z);
    dq.dual = 0.5f * t * r;
}
void TransformDualQuat::SetRotation(const glm::quat& rotation) {
    moved();
    glm::vec3 position = GetPosition();
    dq.real = glm::normalize(rotation);
    SetPosition(position);
}
void TransformDualQuat::SetScale(const glm::vec3& scale) {}

void TransformDualQuat::Move(const glm::vec3& offset) { }
void TransformDualQuat::Move(glm::vec3 direction, float distance) { }

void TransformDualQuat::Rotate(const glm::quat& deltaRotation) { }
