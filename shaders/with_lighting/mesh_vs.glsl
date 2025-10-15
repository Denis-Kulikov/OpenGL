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

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
    vec4 worldPos = Model * vec4(aPosition, 1.0);
    vs_out.FragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    vs_out.Normal = normalize(normalMatrix * aNormal);

    vs_out.TexCoord = aTexCoord;

    gl_Position = Projection * View * worldPos;
}
