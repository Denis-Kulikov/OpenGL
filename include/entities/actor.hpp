#pragma once

#include <iostream>
#include "../object/component/component.hpp"

class Actor
{
public:
    Actor();
    ~Actor();

    virtual std::string GetName() const = 0;

    void Render() const;
    void updateAnimation();

    void Spawn(Transform &startTransform);
    glm::vec3 GetDirection() const; // направление объекта не зависимо от направления модели
    void SetDirection(const glm::vec3 &_direction);

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

    Component *rootComponent = nullptr;
protected:
    glm::vec3 direction = {0, 0, 0};
    float speed;
    inline static std::string name = "NONE";
};
