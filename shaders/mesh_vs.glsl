#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
    TexCoord = aTexCoords;

    gl_Position = Projection * View * Model * vec4(aPosition, 1.0);
}
