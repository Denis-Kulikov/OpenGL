#pragma once
#include <object/transform/transform_dual_quat.hpp>
#include <object/material/material.hpp>

class ComponentDualQuat {
public:
    ComponentDualQuat(TransformDualQuat *transform);
    ~ComponentDualQuat();

    virtual void UpdateQuats(TransformDualQuat* parentTransform = nullptr);
    virtual void Update(float deltaTime) {};
    virtual void UpdateTree(float deltaTime);

    virtual void Render() const {};
    void RenderTree() const;

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetLocalScale() const;

    glm::vec3 GetGlobalPosition() const;
    glm::quat GetGlobalRotation() const;
    glm::vec3 GetGlobalScale() const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    TransformDualQuat *GetTransform() { return globalTransform; }
    TransformDualQuat *GetLocalTransform() { return localTransform; }
    ComponentDualQuat *GetParent() const { return parent; }
    std::vector<ComponentDualQuat*>& GetChildren() { return children; }
    void AddChild(ComponentDualQuat* child);

    std::vector<ComponentDualQuat*> children;
    ComponentDualQuat *parent = nullptr;

// protected:
    TransformDualQuat* globalTransform = nullptr;
    TransformDualQuat* localTransform = nullptr;
    glm::vec3 scale;
};
