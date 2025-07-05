#include <object/transform/transform_dual_quat.hpp>


TransformDualQuat::TransformDualQuat() : dq_real(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), dq_dual(glm::quat(0, 0, 0, 0)){}

glm::quat TransformDualQuat::GetPositionQuat() const {
    return dq_dual;
}
glm::vec3 TransformDualQuat::GetPosition() const {
    glm::quat t = 2.0f * dq_dual * glm::conjugate(dq_real);
    return glm::vec3(t.x, t.y, t.z);
}
glm::quat TransformDualQuat::GetRotation() const { return dq_real; }
glm::vec3 TransformDualQuat::GetScale()    const { return glm::vec3(1.f); }

void TransformDualQuat::SetPosition(const glm::quat& position) { moved(); dq_dual = position; }
void TransformDualQuat::SetPosition(const glm::vec3& position) {
    moved();
    glm::quat r = dq_real;
    glm::quat t(0, position.x, position.y, position.z);
    dq_dual = 0.5f * t * r;
}
void TransformDualQuat::SetRotation(const glm::quat& rotation) {
    moved();
    glm::vec3 position = GetPosition();
    dq_real = glm::normalize(rotation);
    SetPosition(position);
}
void TransformDualQuat::SetScale(const glm::vec3& scale) {}

void TransformDualQuat::Move(const glm::vec3& offset) { }
void TransformDualQuat::Move(glm::vec3 direction, float distance) { }

void TransformDualQuat::Rotate(const glm::quat& deltaRotation) { }
