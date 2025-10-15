#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
    vec3 viewPos;
};

uniform mat4 Model;

out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;

    gl_Position = Projection * View * Model * vec4(aPosition, 1.0);
}
