#version 430 core

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
    int shadowIndex;
    float padding1;
    float padding2;
    float padding3;
};

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
    vec3 viewPos;
};

layout(std430, binding = 1) buffer Lights {
    int numPointLights;
    int numSpotLights;
    int numDirectionalLights;
    int padding1;
    PointLight pointLights[128];
};

uniform sampler2D albedoMap;
uniform samplerCube shadowCubeMap; // пока один источник
uniform float farPlanes;
uniform float ambientStrength;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosDirLightSpace;
} fs_in;

out vec4 FragColor;

float SampleShadowCube(vec3 fragPos, vec3 lightPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float closestDepth = texture(shadowCubeMap, normalize(fragToLight)).r * farPlanes;

    float bias = 0.04;

    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec3 albedo = texture(albedoMap, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 color = vec3(0.0);

    for (int i = 0; i < numPointLights; ++i) {
        PointLight light = pointLights[i];

        vec3 lightDir = normalize(light.position - fs_in.FragPos);
        float distance = length(light.position - fs_in.FragPos);

        // Затухание света
        float attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);
        attenuation *= attenuation; // мягкое падение

        // Базовое диффузное освещение
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * light.color * light.intensity;

        // Расчёт тени
        float shadow = SampleShadowCube(fs_in.FragPos, light.position);

        // Амбиент и финальный свет
        vec3 ambient = ambientStrength * light.color;
        vec3 lighting = ambient + (1.0 - shadow) * diffuse * attenuation;

        color += lighting;
    }

    FragColor = vec4(albedo * color, 1.0);
}
