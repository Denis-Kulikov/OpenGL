#pragma once

#include "actor.hpp"
#include "../render/camera.hpp"

template <typename Derived>
class Pawn : public Actor<Derived>
{
public:
    Pawn(const std::string &path)
        : Actor<Derived>(path),
          cameraPos(Vector3<GLfloat>(0.0f, 6.0f, 15)),
          camera(nullptr)
    {
    }

    void Teleport(const Vector3<GLfloat> newPosition) {
        this->trans.WorldPos = newPosition;
    }

    void Move(const Vector3<GLfloat> offset) {
        this->trans.Move(offset);
    }

    void Move(const GLfloat distance, const Vector3<GLfloat> direction) {
        this->trans.Move(distance, direction);
    }

    void MoveForward() {
        this->trans.Move(this->speed, this->direction);
    }

    void MoveForward(const GLfloat distance) {
        this->trans.Move(distance, this->direction);
    }
    
    template<typename T> // ***
    void MoveTowards(T* targetPawn, GLfloat speed) {
        Vector3<GLfloat> direction = targetPawn->GetTransform()->WorldPos - this->trans.WorldPos;
        Move(speed, direction);
    }

    void Rotate(const Vector3<GLfloat> _rotate) {
        this->trans.Rotate = _rotate;
    }

    void AddRotate(const Vector3<GLfloat> _rotate) {
        this->trans.Rotate += _rotate;
    }

    void SetScale(const Vector3<GLfloat> _scale) {
        this->trans.Scale = _scale;
    }

    void MultiplyScale(const Vector3<GLfloat> _scale) {
        this->trans.Scale *= _scale;
    }

    void attachCamera(Camera *_camera)
    {
        camera = _camera;
    }

    bool createCamera(int width, int height)
    {
        auto _camera = new Camera();

        Vector3<GLfloat> CameraPos(0.0f, 3.0f, 15);
        Vector3<GLfloat> CameraTarget(0.0f, -0.4f, -1.0f);
        Vector3<GLfloat> CameraUp(0.0f, 1.0f, 0.0f);

        _camera->SetCamera(CameraPos, CameraTarget, CameraUp);
        _camera->SetPerspectiveProj(60.0f, width, height, 0.5f, 1000.0f);

        attachCamera(_camera);

        return true;
    }

    void UpdateCameraPos()
    {
        this->camera->Params.WorldPos = cameraPos + this->trans.WorldPos;
    }


    Camera *GetCamera()
    {
        return camera;
    }

    Vector3<GLfloat> *GetCameraPosition()
    {
        return &cameraPos;
    }

    GLfloat GetSpeed()
    {
        return speed;
    }

protected:
    Camera *camera;
    Vector3<GLfloat> cameraPos;
    GLfloat speed = 0.3;
};

