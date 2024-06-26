#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

class Pawn : public Actor
{
public:
    Pawn(const std::string &path, const size_t SkeletSize);
    ~Pawn();

    void Teleport(const Vector3<GLfloat> newPosition);
    void Move(const Vector3<GLfloat> offset);
    void Move(const GLfloat distance, const Vector3<GLfloat> direction);
    void MoveForward();
    void MoveForward(const GLfloat distance);
    
    template<typename T> // ***
    void MoveTowards(T* targetPawn, GLfloat speed) {
        Vector3<GLfloat> direction = targetPawn->GetTransform()->WorldPos - this->trans.WorldPos;
        Move(speed, direction);
    }

    void Rotate(const Vector3<GLfloat> _rotate);
    void AddRotate(const Vector3<GLfloat> _rotate);
    void SetScale(const Vector3<GLfloat> _scale);
    void MultiplyScale(const Vector3<GLfloat> _scale);
    void attachCamera(Camera *_camera);
    bool createCamera(int width, int height);
    Camera *GetCamera();
    Vector3<GLfloat> GetCameraPosition();
    GLfloat GetSpeed();

protected:
    Camera *camera;
    GLfloat speed = 0.3;
};

