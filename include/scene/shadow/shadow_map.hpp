#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

glm::mat4 LookAtLH(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
glm::mat4 PerspectiveLH(float fovY, float aspect, float near, float far);
glm::mat4 OrthoLH(float left, float right, float bottom, float top, float near, float far);

enum ShadowMapType {
    PERSPECTIVE,
    ORTHO
};

class ShadowMap {
public:
    ShadowMap(GLsizei SHADOW_WIDTH = 512, GLsizei SHADOW_HEIGHT = 512)
        : SHADOW_WIDTH(SHADOW_WIDTH), SHADOW_HEIGHT(SHADOW_HEIGHT)
    {}

    GLuint GetDepthMap() const { return map; }
    GLuint GetBuffer() const { return FBO; }
    float GetNear() const { return near; }
    float GetFar() const { return far; }

protected:
    GLuint FBO = 0;
    GLuint map = 0;
    GLsizei SHADOW_WIDTH;
    GLsizei SHADOW_HEIGHT;

    float near = 0.1f;
    float far = 128.0f;
};
