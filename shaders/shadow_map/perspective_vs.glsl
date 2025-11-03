#version 330 core
layout (location = 0) in vec3 aPosition;

uniform mat4 ShadowProj;
uniform mat4 Model;

out vec4 FragPosWorld;

void main()
{
    FragPosWorld = Model * vec4(aPosition, 1.0);
    gl_Position = ShadowProj * FragPosWorld;
}
