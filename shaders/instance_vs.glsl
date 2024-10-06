#version 330 core 

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec3 instancePosition; // Положение инстанса куба
uniform mat4 commonMatrix;

void main() {
    vec4 worldPosition = vec4(VertexPosition + instancePosition, 1.0); // Добавляем смещение инстанса
    gl_Position = commonMatrix * worldPosition;
}

