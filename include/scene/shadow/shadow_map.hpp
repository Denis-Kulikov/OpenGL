#pragma once
#include <GL/glew.h>

enum ShadowMapType {
    SHADOW_CUBE_MAP
};

class ShadowMap {
public:
    ShadowMap(GLsizei resolution = 512)
        : SHADOW_SIZE(resolution)
    {}

    GLuint GetDepthMap() const { return map; }
    GLuint GetBuffer() const { return FBO; }

protected:
    GLuint FBO = 0;
    GLuint map = 0;
    GLsizei SHADOW_SIZE;
};