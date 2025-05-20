#pragma once
#include "actor.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path);

    void Teleport(const glm::vec3 newPosition);
    void Move(const glm::vec3 offset);
    void Move(const glm::vec3 direction, const float distance);
    void MoveForward(const float distance);
    void MoveForward();
    void MoveTowards(const glm::vec3 target, const float distance);
    void Rotate(const glm::vec3 rotate);
    void AddRotate(const glm::vec3 rotate);
    void SetScale(const glm::vec3 scale);
    void MultiplyScale(const glm::vec3 scale);
    float GetSpeed() const;

protected:
    float speed;
};

