#pragma once

#include <filesystem>
#include "../lib-project/lib.hpp"
#include "../object/objectTransform.hpp"
#include "../object/renderableObject.hpp"
#include "../mesh/mesh.hpp"

#include "../object/component/component.hpp"

class Actor : public RenderableObject
{
public:
    Actor(const std::string &path);
    ~Actor();

    struct Actor_rdata {
        std::vector<aiMatrix4x4> *BonesTransforms;
        Mesh *mesh;
    };

    virtual std::string GetName() const = 0;
    virtual Mesh *GetMesh() const = 0;

    void Render(void *RenderData) const override;
    void updateAnimation();

    void Spawn(Transform &startTransform);
    objectTransform *GetTransform();
    glm::vec3 GetDirection() const; // направление объекта не зависимо от направления модели
    void SetDirection(const glm::vec3 &_direction);


    Component *rootComponent = nullptr;
protected:
    bool loadActor(const std::string &path);

    glm::vec3 direction = {0, 0, 0};
    objectTransform transform;

    inline static Mesh *mesh = nullptr;
    inline static std::string name = "NONE";
};
