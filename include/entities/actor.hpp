#pragma once

#include <iostream>
#include "../object/component/component.hpp"

class Actor
{
public:
    Actor(const std::string &path);
    ~Actor();

    virtual std::string GetName() const = 0;

    void Render() const;
    void updateAnimation();

    void Spawn(Transform &startTransform);
    glm::vec3 GetDirection() const; // направление объекта не зависимо от направления модели
    void SetDirection(const glm::vec3 &_direction);

    Component *rootComponent = nullptr;
protected:
    glm::vec3 direction = {0, 0, 0};
    inline static std::string name = "NONE";
};
