#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path);

    void Teleport(const glm::vec3 newPosition);
    void Move(const glm::vec3 offset);
    void Move(const GLfloat distance, const glm::vec3 direction);
    void MoveForward(const GLfloat distance);
    void MoveForward();
    void MoveTowards(const glm::vec3 target, const GLfloat speed);
    void Rotate(const glm::vec3 rotate);
    void AddRotate(const glm::vec3 rotate);
    void SetScale(const glm::vec3 scale);
    void MultiplyScale(const glm::vec3 scale);
    GLfloat GetSpeed() const;
    GLfloat GetYaw() const;
    GLfloat GetPitch() const;
    void SetYaw(const GLfloat new_yaw);
    void SetPitch(const GLfloat new_pitch);

protected:
    GLfloat speed = 30.0;
    GLfloat yaw = 0;
    GLfloat pitch = 0;
};

