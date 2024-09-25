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

uniform mat4 gBones[128];
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];

    Normal0 = mat3(transpose(inverse(mat3(BoneTransform)))) * aNormal;
    TexCoord0 = aTexCoords;
    WorldPos0 = (BoneTransform * vec4(aPosition, 1.0)).xyz;

    gl_Position = Projection * View * BoneTransform * vec4(aPosition, 1.0);
}
