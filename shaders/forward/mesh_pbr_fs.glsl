#version 430 core

const float PI = 3.1415926535898;

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

    float bias = 0.01;
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

    float bias = 0.01;
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

vec3 saturate(vec3 value) {
    return clamp(value, 0.0, 1.0);
}
float saturate(float value) {
    return clamp(value,0.0,1.0);
}

float GGX_PartialGeometry(float cosThetaN, float alpha) {
    float cosTheta_sqr = saturate(cosThetaN*cosThetaN);
    float tan2 = ( 1 - cosTheta_sqr ) / cosTheta_sqr;
    float GP = 2 / ( 1 + sqrt( 1 + alpha * alpha * tan2 ) );
    return GP;
}

float GGX_Distribution(float cosThetaNH, float alpha) {
    float alpha2 = alpha * alpha;
    float NH_sqr = saturate(cosThetaNH * cosThetaNH);
    float den = NH_sqr * alpha2 + (1.0 - NH_sqr);
    return alpha2 / ( PI * den * den );
}

vec3 FresnelSchlick(vec3 F0, float cosTheta) {
    return F0 + (1.0 - F0) * pow(1.0 - saturate(cosTheta), 5.0);
}

vec3 CookTorrance_GGX(vec3 albedo, vec3 n, vec3 l, vec3 v, vec3 f0) {
    n = normalize(n);
    v = normalize(v);
    l = normalize(l);
    vec3 h = normalize(v+l);

    float NL = dot(n, l);
    if (NL <= 0.0) return vec3(0.0, 0.0, 0.0);
    float NV = dot(n, v);
    if (NV <= 0.0) return vec3(0.0, 0.0, 0.0);
    float NH = dot(n, h);
    float HV = dot(h, v);
    
    float roug_sqr = roughness*roughness;
    
    float G = GGX_PartialGeometry(NV, roug_sqr) * GGX_PartialGeometry(NL, roug_sqr);
    float D = GGX_Distribution(NH, roug_sqr);
    vec3 F = FresnelSchlick(f0, HV);

    vec3 specK = G*D*F*0.25/(NV+0.001);    
    vec3 diffK = (1.0 - metallic) * saturate(1.0-F);
    return albedo * diffK * NL + specK * specularStrength;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

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
        vec3 V = viewPos - fs_in.FragPos;
        vec3 N = fs_in.Normal;
        vec3 L = light.position - fs_in.FragPos;

        float distance = length(light.position - fs_in.FragPos);
        float attenuation = clamp(1.0 - distance / light.radius, 0.0, 1.0);
        attenuation *= attenuation;

        vec3 ambient = ambientStrength * albedo * dirLights.color;

        vec3 specK = CookTorrance_GGX(albedo, N, L, V, F0);
        vec3 lighting = specK * dirLights.intensity;

        float shadow = SampleShadowCube(fs_in.FragPos, light.position);

        color += (ambient + lighting) * (1.0 - shadow) * attenuation * light.color;

        // PointLight light = pointLights[i];
        // vec3 lightDir = normalize(light.position - fs_in.FragPos);
        // vec3 reflectDir = reflect(-lightDir, normal);
        // float distance = length(light.position - fs_in.FragPos);

        // float attenuation = clamp(1.0 - distance / light.radius, 0.0, 1.0);
        // attenuation *= attenuation;

        // float diff = max(dot(normal, lightDir), 0.0);

        // float shininess = mix(4.0, 128.0, 1.0 - roughness); // roughness обратный
        // float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

        // float NdotL = max(dot(normal, lightDir), 0.0);
        // vec3 halfDir = normalize(lightDir + viewDir);
        // vec3 F = fresnelSchlickRoughness(max(dot(halfDir, viewDir), 0.0), F0, roughness);
        // vec3 kS = F;
        // vec3 kD = (1.0 - kS) * (1.0 - metallic);

        // vec3 ambient = kD * ambientStrength * MAX_AMBIENT * albedo * light.color;
        // vec3 diffuse = kD * NdotL * diff * MAX_DIFFUSE * albedo * light.color;
        // vec3 specular = kS * specularStrength * spec * MAX_SPECULAR * light.color;

        // if (hasSpecularMap)
        //     specular *= texture(specularMap, fs_in.TexCoord).rgb;

        // float shadow = SampleShadowCube(fs_in.FragPos, light.position);
        // vec3 lighting = ambient + ((diffuse + specular) * (1.0 - shadow)) * light.intensity * attenuation;

        // color += lighting;
    }

    // directional light (по аналогии)
    if (numDirectionalLights > 0) {
        vec3 V = viewPos - fs_in.FragPos;
        vec3 N = fs_in.Normal;
        vec3 L = -dirLights.direction;

        vec3 ambient = ambientStrength * albedo * dirLights.color;

        vec3 specK = CookTorrance_GGX(albedo, N, L, V, F0);
        vec3 lighting = specK * dirLights.intensity;

        float shadow = ShadowCalculation(shadowMapDirLight, fs_in.FragPosDirLightSpace, N, L);

        color += (ambient + lighting) * (1.0 - shadow);
    }

    // color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

