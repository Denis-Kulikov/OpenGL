#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
    vec3 viewPos;
};

layout(std430, binding = 2) buffer BonesData {
    mat4 Bones[];
};
uniform mat4 Model;

out vec2 TexCoord;

void main()
{
    mat4 BoneTransform = Bones[aBoneIDs[0]] * aWeights[0];
    BoneTransform     += Bones[aBoneIDs[1]] * aWeights[1];
    BoneTransform     += Bones[aBoneIDs[2]] * aWeights[2];
    BoneTransform     += Bones[aBoneIDs[3]] * aWeights[3];

    TexCoord = aTexCoord;

    gl_Position = Projection * View * Model * BoneTransform * vec4(aPosition, 1.0);
}
