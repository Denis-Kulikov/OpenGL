#pragma once
#include "../lib-project/lib.hpp"
#include "../lib-project/math_3d.hpp"

enum AXES {
    X,
    Y,
    Z
};

class objectTransform 
{
public:
    objectTransform() {
        WorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        Rotate   = glm::vec3(0.0f, 0.0f, 0.0f);
        Scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::vec3 GetWorldPos() const;
    glm::vec3 GetRotate() const;
    glm::vec3 GetScale() const;

    void SetWorldPos(const GLfloat &WorldPosX, const GLfloat &WorldPosY, const GLfloat &WorldPosZ);
    void SetWorldPos(const glm::vec3 &v_WorldPos);

    float GetRotateTowards(const glm::vec3 &target_pos);
    void SetRotate(const GLfloat &RotateX, const GLfloat &RotateY, const GLfloat &RotateZ);
    void SetRotate(const glm::vec3 &v_Rotate);
    
    void SetScale(const GLfloat &ScaleX, const GLfloat &ScaleY, const GLfloat &ScaleZ);
    void SetScale(const glm::vec3 &v_Scale);

    void SetTransform(const objectTransform &trans);

    void Move(const GLfloat &X, const GLfloat &Y, const GLfloat &Z);
    void Move(const glm::vec3 offset);
    void Move(const GLfloat distance, const glm::vec3 direction);
    void MoveForward(const GLfloat distance, const enum AXES axis);

    void AddRotate(const glm::vec3 _rotate);

    void MultiplyScale(const glm::vec3 _scale);

    glm::vec3 GetForwardVector(const enum AXES axis) const;

    friend std::ostream& operator<<(std::ostream& os, const objectTransform& transform);

    glm::vec3 WorldPos;
    glm::vec3 Rotate;
    glm::vec3 Scale;
};

