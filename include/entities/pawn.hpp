#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

template <typename Derived>
class Pawn : public Actor<Derived>
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


    void attachCamera(Camera *_camera);
    bool createCamera(int width, int height);
    void UpdateCameraPos();

    void MoveTowards(Pawn* targetPawn, GLfloat speed);

    Camera *camera;
    Vector3<GLfloat> cameraPos;

protected:

};
