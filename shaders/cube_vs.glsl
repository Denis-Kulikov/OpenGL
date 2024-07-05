#version 330 core

layout(location = 0) in vec3 VertexPosition;

uniform mat4 gWorld;

void main() {
    gl_Position = gWorld * vec4(VertexPosition, 1.0);
}