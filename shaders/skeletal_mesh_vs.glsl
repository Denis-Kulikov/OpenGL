#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
};

layout(std430, binding = 1) buffer Lights {
    vec4 position[];
    vec4 color[];
    int count;
};

out vec2 TexCoord;

uniform mat4 gBones[128];
// uniform mat4 Projection;
// uniform mat4 View;
uniform mat4 Model;

void main()
{
    mat4 BoneTransform = gBones[aBoneIDs[0]] * aWeights[0];
    BoneTransform     += gBones[aBoneIDs[1]] * aWeights[1];
    BoneTransform     += gBones[aBoneIDs[2]] * aWeights[2];
    BoneTransform     += gBones[aBoneIDs[3]] * aWeights[3];

    TexCoord = aTexCoords;

    gl_Position = Projection * View * Model * BoneTransform * vec4(aPosition, 1.0);
}
