#ifndef MATRICES_GLSL
#define MATRICES_GLSL

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
    vec3 viewPos;
};

#endif