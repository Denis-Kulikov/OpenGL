#version 330 core

out vec4 FragColor;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

struct DirectionalLight
{
    vec3 Color;
    vec3 Direction;
    float AmbientIntensity;
    float DiffuseIntensity;
};

uniform DirectionalLight gDirectionalLight;
uniform vec3 gEyeWorldPos;
uniform sampler2D gTextureSampler;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

void main()
{
    vec3 Normal = normalize(Normal0);
    
    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;
    vec3 LightDirection = -gDirectionalLight.Direction;

    float DiffuseFactor = dot(Normal, LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(gDirectionalLight.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;
        }
    }

    FragColor = texture2D(gTextureSampler, TexCoord0.xy) * (AmbientColor + DiffuseColor + SpecularColor);
}

// void main()
// {
//     vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;
//     float DiffuseFactor = dot(normalize(Normal0), -gDirectionalLight.Direction);
//     vec4 DiffuseColor = DiffuseFactor > 0 ? vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.DiffuseIntensity * DiffuseFactor : vec4(0, 0, 0, 0);
//     FragColor = texture2D(TextureSampler, TexCoords.xy) * (AmbientColor + DiffuseColor);
// }
