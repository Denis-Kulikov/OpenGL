#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoord;

out vec2 TexCoordOut;

uniform mat4 gWorld;

void main() {
    gl_Position = gWorld * vec4(VertexPosition, 1.0);
    TexCoordOut = TexCoord;
}
