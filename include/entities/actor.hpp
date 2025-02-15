#pragma once

#include <filesystem>
#include "../object/objectTransform.hpp"
#include "../mesh/mesh.hpp"

#include "../object/component/component.hpp"

class Actor
{
public:
    Actor(const std::string &path);
    ~Actor();

    virtual std::string GetName() const = 0;
    virtual Mesh *GetMesh() const = 0;

    void Render() const;
    void updateAnimation();

    void Spawn(Transform &startTransform);
    objectTransform *GetTransform();
    glm::vec3 GetDirection() const; // направление объекта не зависимо от направления модели
    void SetDirection(const glm::vec3 &_direction);


    Component *rootComponent = nullptr;
protected:
    glm::vec3 direction = {0, 0, 0};
    objectTransform transform;

    inline static Mesh *mesh = nullptr;
    inline static std::string name = "NONE";
};
