#include <object/transform/transform.hpp>


Transform::Transform() : Position(0.0f), Rotation(glm::quat(1, 0, 0, 0)), Scale(1.0f) {}

void Transform::UpdateTransform() {
    glm::mat3 rotMat = glm::mat3(
        glm::vec3(matrix[0]),
        glm::vec3(matrix[1]),
        glm::vec3(matrix[2])
    );

    Transform::SetPosition(glm::vec3(matrix[3]));
    Transform::SetRotation(glm::quat_cast(rotMat));
    Transform::SetScale(glm::vec3(glm::length(matrix[0]),
                                  glm::length(matrix[1]),
                                  glm::length(matrix[2])));
}

void Transform::UpdateMatrix() {
    if (!dirty) return;
    glm::mat4 T = glm::translate(glm::mat4(1.0f), GetPosition());
    glm::mat4 R = glm::mat4_cast(GetRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), GetScale());
    matrix = T * R * S;
    dirty = false;
}

glm::vec3 Transform::GetPosition() const { return Position; }
glm::quat Transform::GetRotation() const { return Rotation; }
glm::vec3 Transform::GetScale()    const { return Scale; }

void Transform::SetMatrix(const glm::mat4x3& mat) { matrix = mat; UpdateTransform(); }

void Transform::SetPosition(const glm::vec3& position) { moved(); Position = position; }
void Transform::SetRotation(const glm::quat& rotation) { moved(); Rotation = rotation; }
void Transform::SetScale(const glm::vec3& scale)       { moved(); Scale = scale; }

void Transform::Move(const glm::vec3& offset) { moved(); Position += offset; }
void Transform::Move(glm::vec3 direction, float distance) {
    moved();
    if (glm::length(direction) != 0) {
        direction = glm::normalize(direction);
        Move(direction * distance);
    }
}

void Transform::Rotate(const glm::quat& deltaRotation) { moved(); Rotation = glm::normalize(deltaRotation * Rotation); }
void Transform::RotateAround(const glm::vec3& axis, float angle) { 
    moved();
    glm::quat delta = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    Rotate(delta);
}

glm::vec3 Transform::GetForwardVector() const { return glm::normalize(Rotation * glm::vec3(0.0f, 0.0f, -1.0f)); }
glm::vec3 Transform::GetUpVector()      const { return glm::normalize(Rotation * glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 Transform::GetRightVector()   const { return glm::normalize(Rotation * glm::vec3(1.0f, 0.0f, 0.0f)); }