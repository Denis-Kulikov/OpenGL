#pragma once

#include <iostream>
#include "../object/component/component.hpp"
#include "../object/component/component_dual_quat.hpp"

#define ACTOR_BASE                                                              \
public:                                                                         \
    static void Initialize();                                                   \
    const std::string& GetOriginalName() const override;                        \
                                                                                \
protected:                                                                      \
    void IncrementInstanceCount() override;                                     \
    unsigned int GetInstanceCount() const override;                             \
                                                                                \
    inline static std::string originalName;                                     \
    inline static uint32_t instanceCount = 0;

#define ACTOR_BASE_CPP(NAME)                                                    \
    const std::string& NAME::GetOriginalName() const { return originalName; }   \
    void NAME::IncrementInstanceCount() { ++instanceCount; }                    \
    uint32_t NAME::GetInstanceCount() const { return instanceCount; }

#define ACTOR_SET_NAME                                                          \
    IncrementInstanceCount();                                                   \
    name = GetOriginalName() + "_" + std::to_string(GetInstanceCount());


class Actor
{
public:
    Actor();
    ~Actor();

    void SetName(const std::string& newName);
    const std::string& GetName() const;
    virtual const std::string& GetOriginalName() const = 0;

    void Render() const;
    void RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const;

    glm::vec3 GetDirection() const; // направление объекта не зависимо от направления модели
    void SetDirection(const glm::vec3 &_direction);

    void Teleport(const glm::vec3 newPosition);
    void Move(const glm::vec3 offset);
    void Move(const glm::vec3 direction, const float distance);
    void MoveForward(const float distance);
    void MoveForward();
    void MoveTowards(const glm::vec3 target, const float distance);
    void SetRotation(const glm::vec3 rotate);
    void AddRotate(const glm::vec3 rotate);
    void SetScale(const glm::vec3 scale);
    void MultiplyScale(const glm::vec3 scale);
    float GetSpeed() const;

    template<typename C, typename... Args>
    C* CreateComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");

        auto comp = new C(std::forward<Args>(args)...);
        comp->owner = this;

        return comp;
    }

    Component *rootComponent = nullptr;
    ComponentDualQuat *rootDualQuat = nullptr;
    
protected:
    virtual void IncrementInstanceCount() = 0;
    virtual uint32_t GetInstanceCount() const = 0;


    glm::vec3 direction = {0, 0, 0};
    float speed;
    inline static std::string originalName = "NONE";
    std::string name;
    bool castsShadow = true; 
};
