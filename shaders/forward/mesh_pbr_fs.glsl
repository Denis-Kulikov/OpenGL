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

struct DirectionalLight {
    vec3 direction;
    float padding1;
    vec3 color;
    float intensity;
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
    // SpotLight spotLights[32];
    DirectionalLight dirLights;
};

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D shadowMapDirLight;
uniform samplerCube shadowCubeMap;
uniform bool hasSpecularMap;
uniform vec3 lightPos;
uniform float farPlanes;

uniform float roughness;
uniform float metallic;
uniform float ambientStrength;
uniform float specularStrength;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosDirLightSpace;
} fs_in;

out vec4 FragColor;

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;

    float bias = 0.05;
    float shadow = 0;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); // размер пикселя в карте теней
    int samples = 1; // радиус выборки = 2 -> 5x5 выборок (при желании увеличить до 3)

    for (int x = -samples; x <= samples; ++x) {
        for (int y = -samples; y <= samples; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            float currentDepth = projCoords.z - bias;
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= pow((samples * 2 + 1), 2); // нормализуем

    return shadow;
}

float SampleShadowCube(vec3 fragPos, vec3 lightPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;

    float bias = 0.05;
    float offset = 0.005;

    vec3 sampleOffsetDirections[6] = vec3[]
    (
        vec3( 1,  0,  0),
        vec3(-1,  0,  0),
        vec3( 0,  1,  0),
        vec3( 0, -1,  0),
        vec3( 0,  0,  1),
        vec3( 0,  0, -1)
    );

    for (int i = 0; i < 6; ++i)
    {
        vec3 sampleDir = normalize(fragToLight + sampleOffsetDirections[i] * offset);
        float closestDepth = texture(shadowCubeMap, sampleDir).r * farPlanes;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= 6.0;

    return shadow;
}

// float SampleShadowCube(vec3 fragPos, vec3 lightPos)
// {
//     vec3 fragToLight = fragPos - lightPos;
//     float currentDepth = length(fragToLight);
//     float shadow = 0.0;

//     float bias = 0.04;
//     float offset = 0.1;

//     for (int x = -1; x <= 1; ++x)
//     for (int y = -1; y <= 1; ++y)
//     for (int z = -1; z <= 1; ++z) {
//         vec3 sampleDir = normalize(fragToLight + vec3(x, y, z) * offset);
//         float closestDepth = texture(shadowCubeMap, sampleDir).r * farPlanes;
//         if (currentDepth - bias > closestDepth)
//             shadow += 1.0;
//     }
//     shadow /= 27.0;

//     return shadow;
// }


void main()
{
    vec3 albedo = texture(albedoMap, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 color = vec3(0.0);

    const float MAX_AMBIENT = 1.0;
    const float MAX_DIFFUSE = 1.0;
    const float MAX_SPECULAR = 1.0;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    for (int i = 0; i < numPointLights; ++i) {
        PointLight light = pointLights[i];
        vec3 lightDir = normalize(light.position - fs_in.FragPos);
        float distance = length(light.position - fs_in.FragPos);

        float attenuation = clamp(1.0 - distance / light.radius, 0.0, 1.0);
        attenuation *= attenuation;

        float diff = max(dot(normal, lightDir), 0.0);

        vec3 ambient = ambientStrength * MAX_AMBIENT * albedo * light.color;
        vec3 diffuse = (1.0 - metallic) * diff * MAX_DIFFUSE * albedo * light.color;
        vec3 specular = vec3(0.0);

        if (diff > 0.0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float shininess = mix(4.0, 128.0, 1.0 - roughness); // roughness обратный
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            specular = specularStrength * spec * MAX_SPECULAR * light.color;
            if (hasSpecularMap)
                specular *= texture(specularMap, fs_in.TexCoord).rgb;
        }

        float shadow = SampleShadowCube(fs_in.FragPos, light.position);
        vec3 lighting = ambient + ((diffuse + specular) * (1.0 - shadow)) * light.intensity * attenuation;

        color += lighting;
    }

    // directional light (по аналогии)
    if (numDirectionalLights > 0) {
        DirectionalLight light = dirLights;
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 ambient = ambientStrength * MAX_AMBIENT * albedo * light.color;
        vec3 diffuse = (1.0 - metallic) * diff * MAX_DIFFUSE * albedo * light.color;
        vec3 specular = vec3(0);

        if (diff > 0.0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float shininess = mix(4.0, 128.0, 1.0 - roughness);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            specular = specularStrength * spec * MAX_SPECULAR * light.color;
            if (hasSpecularMap)
                specular *= texture(specularMap, fs_in.TexCoord).rgb;
        }

        float shadow = ShadowCalculation(shadowMapDirLight, fs_in.FragPosDirLightSpace, normal, lightDir);
        vec3 lighting = (ambient + (diffuse + specular) * (1.0 - shadow)) * light.intensity;

        color += lighting;
    }

    FragColor = vec4(color, 1.0); // ✅ исправлено
}

