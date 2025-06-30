#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec2 TexCoord;

uniform mat4x3 gBones[128];
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    mat4x3 BoneTransform = gBones[aBoneIDs[0]] * aWeights[0];
    BoneTransform     += gBones[aBoneIDs[1]] * aWeights[1];
    BoneTransform     += gBones[aBoneIDs[2]] * aWeights[2];
    BoneTransform     += gBones[aBoneIDs[3]] * aWeights[3];

    TexCoord = aTexCoords;

    vec3 skinnedPos = BoneTransform * vec4(aPosition, 1.0);
    vec4 worldPosition = vec4(skinnedPos, 1.0);
    gl_Position = Projection * View * worldPosition;
}
