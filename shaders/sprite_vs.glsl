#version 330 core

uniform mat4 gWorld;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = gWorld * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
}
