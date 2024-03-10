#pragma once

#include "actor.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path);

    void Teleport(const Vector3<GLfloat> newPosition);
    void Move(const Vector3<GLfloat> offset);
    void Move(const GLfloat distance, const Vector3<GLfloat> direction);
    void MoveForward(const GLfloat distance);

    void Rotate(const Vector3<GLfloat> _rotate);
    void AddRotate(const Vector3<GLfloat> _rotate);

    void SetScale(const Vector3<GLfloat> _scale);
    void MultiplyScale(const Vector3<GLfloat> _scale);

protected:

};
