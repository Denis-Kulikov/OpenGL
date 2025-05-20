#include <render/pipeline.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>


glm::mat4 Pipeline::GetModel(const objectTransform& transform) const
{
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));

    glm::mat4 rotateX = glm::rotate(glm::radians(transform.Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::radians(transform.Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateZ = glm::rotate(glm::radians(transform.Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotateMatrix = rotateZ * rotateY * rotateX;

    glm::mat4 translationMatrix = glm::translate(glm::vec3(transform.WorldPos.x, transform.WorldPos.y, transform.WorldPos.z));

    return translationMatrix * rotateMatrix * scaleMatrix;
}

glm::mat4 Pipeline::GetModel(const Transform& transform) const
{
    glm::mat4 scaleMatrix = glm::scale(transform.GetScale());
    glm::mat4 rotateMatrix = glm::mat4_cast(transform.GetRotation());
    glm::mat4 translationMatrix = glm::translate(transform.GetPosition());

    return translationMatrix * rotateMatrix * scaleMatrix;
}
