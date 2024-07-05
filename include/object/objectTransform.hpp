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
        WorldPos = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
        Rotate   = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
        Scale    = Vector3<GLfloat>(1.0f, 1.0f, 1.0f);
    }

    Vector3<GLfloat> GetWorldPos() const;
    Vector3<GLfloat> GetRotate() const;
    Vector3<GLfloat> GetScale() const;

    void SetWorldPos(const GLfloat &WorldPosX, const GLfloat &WorldPosY, const GLfloat &WorldPosZ);
    void SetWorldPos(const Vector3<GLfloat> &v_WorldPos);

    void SetRotate(const GLfloat &RotateX, const GLfloat &RotateY, const GLfloat &RotateZ);
    void SetRotate(const Vector3<GLfloat> &v_Rotate);
    
    void SetScale(const GLfloat &ScaleX, const GLfloat &ScaleY, const GLfloat &ScaleZ);
    void SetScale(const Vector3<GLfloat> &v_Scale);

    void SetTransform(const objectTransform &trans);

    void Move(const GLfloat &X, const GLfloat &Y, const GLfloat &Z);
    void Move(const Vector3<GLfloat> offset);
    void Move(const GLfloat distance, const Vector3<GLfloat> direction);
    void MoveForward(const GLfloat distance, const enum AXES axis);

    void AddRotate(const Vector3<GLfloat> _rotate);

    void MultiplyScale(const Vector3<GLfloat> _scale);

    Vector3<GLfloat> GetForwardVector(const enum AXES axis) const;

    friend std::ostream& operator<<(std::ostream& os, const objectTransform& transform);

    Vector3<GLfloat> WorldPos;
    Vector3<GLfloat> Rotate;
    Vector3<GLfloat> Scale;
};

