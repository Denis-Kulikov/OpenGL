#version 330 core

out vec4 FragColor;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec3 Tangent0;

struct DirectionalLight {
    vec3 Color;
    vec3 Direction;
    float AmbientIntensity;
    float DiffuseIntensity;
};


uniform DirectionalLight gDirectionalLight;
uniform vec3 gEyeWorldPos;
uniform sampler2D gTextureSampler;
uniform sampler2D gNormalMap;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

vec4 CalcLightInternal(vec3 Color, float AmbientIntensity, float DiffuseIntensity, vec3 LightDirection, vec3 Normal) {
    vec4 AmbientColor = vec4(Color, 1.0f) * AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);
 
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
 
    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Color, 1.0f) * DiffuseIntensity * DiffuseFactor;
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;
        }
    }
 
    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 Normal) {
    return CalcLightInternal(gDirectionalLight.Color, gDirectionalLight.AmbientIntensity, gDirectionalLight.DiffuseIntensity, gDirectionalLight.Direction, Normal);
}


vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(Normal0);
    vec3 Tangent = normalize(Tangent0);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(gNormalMap, TexCoord0).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}


void main() {
    vec3 Normal = CalcBumpedNormal();
    Normal = normalize(Normal0);
    vec4 TotalLight = CalcDirectionalLight(Normal);


    FragColor = texture(gTextureSampler, TexCoord0.xy) * TotalLight;
}
