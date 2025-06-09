#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoordOut;

uniform mat4 gWorld;

void main() {
    gl_Position = gWorld * vec4(aVertexPosition, 1.0);
    TexCoordOut = aTexCoord;
}
