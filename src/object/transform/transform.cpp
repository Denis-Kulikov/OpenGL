#include <object/transform/transform.hpp>

Transform::Transform(const Transform &transform)
    : Position(transform.GetPosition()), Rotation(transform.GetRotation()), Scale(transform.GetScale())
{}

Transform::Transform() : Position(0.0f), Rotation(glm::quat(1, 0, 0, 0)), Scale(1.0f) {}

void Transform::UpdateTransform() {
    Transform::SetPosition(glm::vec3(matrix[3]));
    Transform::SetRotation(glm::quat_cast(matrix));
    Transform::SetScale(glm::vec3(glm::length(matrix[0]),
                        glm::length(matrix[1]),
                        glm::length(matrix[2])));
}

void Transform::UpdateMatrix() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), Transform::GetPosition());
    glm::mat4 rotate = glm::mat4_cast(Transform::GetRotation());
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), Transform::GetScale());
    matrix = translate * rotate * scale;
}

glm::vec3 Transform::GetPosition() const { return Position; }
glm::quat Transform::GetRotation() const { return Rotation; }
glm::vec3 Transform::GetScale()    const { return Scale; }

void Transform::SetPosition(const glm::vec3& position) { Position = position; }
void Transform::SetRotation(const glm::quat& rotation) { Rotation = rotation; }
void Transform::SetScale(const glm::vec3& scale)       { Scale = scale; }

void Transform::Move(const glm::vec3& offset) { Position += offset; }
void Transform::Move(glm::vec3 direction, float distance) {
    if (glm::length(direction) != 0) {
        direction = glm::normalize(direction);
        Move(direction * distance);
    }
}

void Transform::Rotate(const glm::quat& deltaRotation) { Rotation = glm::normalize(deltaRotation * Rotation); }
void Transform::RotateAround(const glm::vec3& axis, float angle) { 
    glm::quat delta = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    Rotate(delta);
}

glm::vec3 Transform::GetForwardVector() const { return glm::normalize(Rotation * glm::vec3(0.0f, 0.0f, -1.0f)); }
glm::vec3 Transform::GetUpVector()      const { return glm::normalize(Rotation * glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 Transform::GetRightVector()   const { return glm::normalize(Rotation * glm::vec3(1.0f, 0.0f, 0.0f)); }