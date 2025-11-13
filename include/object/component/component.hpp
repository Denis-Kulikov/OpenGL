#pragma once
#include <object/transform/transform.hpp>
#include <object/transform/rigid_transform.hpp>
#include <ui/visitor/component.hpp>
#include <object/named.hpp>

enum ShadowMapType;

class Actor;

class Component : public Named {
protected:
    Component(const std::string& name, RigidTransform *transform);

public:
    Component(const std::string& name, TransformableMatrix *transform);
    Component(const std::string& name);
    ~Component();

    virtual void UpdateInverse();
    void UpdateInverseTree();
    virtual void UpdateMatrixTree(const glm::mat4x4& parentTR = glm::mat4(1.0f), const glm::mat4x4& parentS = glm::mat4(1.0f));
    virtual void Update(float deltaTime) {};
    virtual void UpdateTree(float deltaTime);

    virtual void Render() const;
    virtual void RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const;
    void RenderTree() const;
    void RenderTreeShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const;

    virtual void Accept(UIVisitorComponent& visitor) = 0;

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetLocalScale() const;

    glm::vec3 GetGlobalPosition() const;
    glm::quat GetGlobalRotation() const;
    glm::vec3 GetGlobalScale() const;

    void FixPosition(const glm::vec3& scaleChange);
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    void Move(const glm::vec3& offset);
    void Move(glm::vec3 direction, float distance);
    void Rotate(const glm::quat& deltaRotation);
    void RotateAround(const glm::vec3& axis, float angle);

    TransformableMatrix *GetTransform() { return globalTransform; }
    TransformableMatrix *GetLocalTransform() { return localTransform; }
    void SetTransform(Transform &new_transformable) { *localTransform = new_transformable; }
    const glm::mat4x3 &GetMatrix() const { return globalTransform->GetMatrix(); }
    Component *GetParent() const { return parent; }
    std::vector<Component*>& GetChildren() { return children; }
    void AddChild(Component* child);

    std::vector<Component*> children;
    Component *parent = nullptr;
    Actor *owner = nullptr;

// protected:
    TransformableMatrix *globalTransform = nullptr;
    TransformableMatrix *localTransform = nullptr;
    glm::vec3 scale;
    glm::vec3 invScale;
    bool castsShadow = false; 
};
