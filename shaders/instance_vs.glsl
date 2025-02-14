#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 3) in vec4 instanceMatrixRow0;
layout (location = 4) in vec4 instanceMatrixRow1;
layout (location = 5) in vec4 instanceMatrixRow2;
layout (location = 6) in vec4 instanceMatrixRow3;

uniform mat4 VP;

void main()
{
    mat4 model = mat4(instanceMatrixRow0, instanceMatrixRow1, instanceMatrixRow2, instanceMatrixRow3);
    gl_Position = VP * model * vec4(aPos, 1.0);
}
