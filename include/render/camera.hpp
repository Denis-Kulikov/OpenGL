#pragma once
#include "../object/transform/transformable.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera
{
public:
    Camera(const Camera &camera) 
    {
        Params.Target   = camera.Params.Target;
        Params.Up       = camera.Params.Up;

        PersProj.FOV    = camera.PersProj.FOV;
        PersProj.Width  = camera.PersProj.Width;
        PersProj.Height = camera.PersProj.Height;
        PersProj.zNear  = camera.PersProj.zNear;
        PersProj.zFar   = camera.PersProj.zFar;
    }
    Camera()
    {
        Params.Target      = glm::vec3(0.0f, 0.0f, 1.0f);
        Params.Up          = glm::vec3(0.0f, 1.0f, 0.0f);
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

    void SetCamera(const glm::vec3& Target, const glm::vec3& Up)
    {
        Params.Target   = Target;
        Params.Up       = Up;
    }

    void SetCamera(const Camera &_camera)
    {
        SetCamera(_camera.Params.Target, _camera.Params.Up);
        SetPerspectiveProj(_camera.PersProj.FOV, _camera.PersProj.Width, _camera.PersProj.Height, _camera.PersProj.zNear, _camera.PersProj.zFar);
    }

    void SetYaw(const GLfloat new_yaw) { yaw = new_yaw; }
    void SetPitch(const GLfloat new_pitch) { pitch = new_pitch; }
    GLfloat GetYaw() const { return yaw; }
    GLfloat GetPitch() const { return pitch; }
    void UpdateTarget() {
        const float pitch_limit = 89.0f;
        float yaw = glm::radians(GetYaw());
        float pitch = glm::radians(glm::clamp(GetPitch(), -pitch_limit, pitch_limit));

        glm::vec3 front;
        front.x = cos(pitch) * sin(yaw);
        front.y = sin(pitch);
        front.z = cos(pitch) * cos(yaw);
        Params.Target = glm::normalize(front);
    }

    struct struct_Params {
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

    GLfloat yaw;
    GLfloat pitch;
};
