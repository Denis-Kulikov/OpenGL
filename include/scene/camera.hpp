#pragma once
#include "../object/transform/transformable.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <GL/glew.h>

glm::mat4 LookAtLH(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
glm::mat4 PerspectiveLH(float fovY, float aspect, float near, float far);
glm::mat4 OrthoLH(float left, float right, float bottom, float top, float near, float far);

class Camera
{
public:
    Camera(const Camera &camera) ;
    Camera();

    void SetPerspectiveProj(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar);
    void SetPerspectiveProj(const Camera &_camera);
    void SetCamera(const glm::vec3& Target, const glm::vec3& Up);
    void SetCamera(const Camera &_camera);
    void SetYaw(const GLfloat new_yaw) { yaw = new_yaw; }
    void SetPitch(const GLfloat new_pitch) { pitch = new_pitch; }
    GLfloat GetYaw() const { return yaw; }
    GLfloat GetPitch() const { return pitch; }
    void UpdateTarget();

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

    GLfloat yaw = -180;
    GLfloat pitch = 0;
};
