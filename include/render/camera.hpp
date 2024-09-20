#pragma once
#include "../object/objectTransform.hpp"

class Camera
{
public:
    Camera()
    {
        Params.WorldPos    = glm::vec3(0.0f, 0.0f, 0.0f);
        Params.Target      = glm::vec3(0.0f, 0.0f, 1.0f);
        Params.Up          = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void attachOwnerTransform(const objectTransform* TransformPtr)
    {
        OwnerTransformPtr = TransformPtr;
    }

    glm::vec3 GetPosition()
    {
        return OwnerTransformPtr == nullptr ? Params.WorldPos : Params.WorldPos + OwnerTransformPtr->GetWorldPos();
    }

    void SetPerspectiveProj(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
    {
        PersProj.FOV    = FOV;
        PersProj.Width  = Width;
        PersProj.Height = Height;
        PersProj.zNear  = zNear;
        PersProj.zFar   = zFar;
    }

    void SetPerspectiveProj(const Camera &_camera)
    {
        PersProj.FOV    = _camera.PersProj.FOV;
        PersProj.Width  = _camera.PersProj.Width;
        PersProj.Height = _camera.PersProj.Height;
        PersProj.zNear  = _camera.PersProj.zNear;
        PersProj.zFar   = _camera.PersProj.zFar;
    }

    void SetCamera(const glm::vec3& WorldPos, const glm::vec3& Target, const glm::vec3& Up)
    {
        Params.WorldPos = WorldPos;
        Params.Target   = Target;
        Params.Up       = Up;
    }

    void SetCamera(const Camera &_camera)
    {
        SetCamera(_camera.Params.WorldPos, _camera.Params.Target, _camera.Params.Up);
        SetPerspectiveProj(_camera.PersProj.FOV, _camera.PersProj.Width, _camera.PersProj.Height, _camera.PersProj.zNear, _camera.PersProj.zFar);
    }

    struct struct_Params {
        glm::vec3 WorldPos;
        glm::vec3 Target;
        glm::vec3 Up;
    } Params;

    struct struct_PersProj {
        GLfloat FOV;
        GLfloat Width;
        GLfloat Height;
        GLfloat zNear;
        GLfloat zFar;
    } PersProj;

    const objectTransform* OwnerTransformPtr = nullptr;
};
