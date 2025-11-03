#version 330 core
layout (location = 0) in vec3 aPosition;

uniform mat4 ShadowProj;
uniform mat4 Model;

void main()
{
    gl_Position = ShadowProj * Model * vec4(aPosition, 1.0);
}
