#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Tangent0;

uniform mat4 gWorld;

void main()
{
    gl_Position = gWorld * vec4(aPosition, 1.0);
    TexCoord0 = aTexCoords;
    Normal0 = (gWorld * vec4(aNormal, 0.0)).xyz;
    Tangent0 = (gWorld * vec4(aTangent, 0.0)).xyz;
    WorldPos0 = (gWorld * vec4(aPosition, 1.0)).xyz;
}
