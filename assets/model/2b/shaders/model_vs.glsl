#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Tangent0;

uniform mat4 gBones[100];
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
    // gl_Position = gWorld * vec4(aPosition, 1.0);

    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0] +
                         gBones[BoneIDs[1]] * Weights[1] +
                         gBones[BoneIDs[2]] * Weights[2] +
                         gBones[BoneIDs[3]] * Weights[3];

    mat4 gWorld = BoneTransform;
    TexCoord0 = aTexCoords;
    Normal0 = (gWorld * vec4(aNormal, 0.0)).xyz;
    Tangent0 = vec3(0, 0, 0);
    WorldPos0 = (gWorld * vec4(aPosition, 1.0)).xyz;

    gl_Position = Projection * View * Model * vec4(aPosition, 1.0);
}
