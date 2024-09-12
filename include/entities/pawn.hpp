#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path, const size_t SkeletSize);

    void Teleport(const Vector3<GLfloat> newPosition);
    void Move(const Vector3<GLfloat> offset);
    void Move(const GLfloat distance, const Vector3<GLfloat> direction);
    void MoveForward(const GLfloat distance);
    void MoveForward();
    
    template<typename T>
    void MoveTowards(T* targetPawn, GLfloat speed) {
        Vector3<GLfloat> direction = targetPawn->GetTransform()->WorldPos - this->trans.WorldPos;
        Move(speed, direction);
    }

    void Rotate(const Vector3<GLfloat> _rotate);
    void AddRotate(const Vector3<GLfloat> _rotate);
    void SetScale(const Vector3<GLfloat> _scale);
    void MultiplyScale(const Vector3<GLfloat> _scale);
    GLfloat GetSpeed() const;
    GLfloat GetYaw() const;
    GLfloat GetPitch() const;
    void SetYaw(GLfloat new_yaw);
    void SetPitch(GLfloat new_pitch);

protected:
    GLfloat speed = 30.0;
    GLfloat yaw = 0;
    GLfloat pitch = 0;
};

