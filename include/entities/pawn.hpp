#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path, const size_t SkeletSize);

    void Teleport(const glm::vec3 newPosition);
    void Move(const glm::vec3 offset);
    void Move(const GLfloat distance, const glm::vec3 direction);
    void MoveForward(const GLfloat distance);
    void MoveForward();
    void MoveTowards(const glm::vec3 target_pos, GLfloat speed) {
        // glm::vec3 direction = target_pos - this->GetTransform()->WorldPos;
        // direction.Normalize();
        // Move(speed, direction);
    }

    void Rotate(const glm::vec3 _rotate);
    void AddRotate(const glm::vec3 _rotate);
    void SetScale(const glm::vec3 _scale);
    void MultiplyScale(const glm::vec3 _scale);
    GLfloat GetSpeed() const;
    GLfloat GetYaw() const;
    GLfloat GetPitch() const;
    void SetYaw(GLfloat new_yaw);
    void SetPitch(GLfloat new_pitch);

protected:
    GLfloat speed = 30.0;
    GLfloat yaw = 0;
    GLfloat pitch = 0;
};

