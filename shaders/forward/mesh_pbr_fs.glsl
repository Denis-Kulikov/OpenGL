#version 430 core

struct PointLight {
    vec4 position;
    vec4 color;
    float radius;
    float padding1;
    float padding2;
    float padding3;
};

struct DirectionalLight {
    vec4 direction;
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
    int numDirectionalLights;
    int numSpotLights;
    int padding1;
    PointLight pointLights[128];
    DirectionalLight dirLights[16];
    // SpotLight spotLights[32];
};

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform bool hasSpecularMap;

// [0; 1] параметры материала
uniform float roughness;  // 0 — зеркало, 1 — матовая
uniform float metallic;   // 0 — пластик, 1 — металл
uniform float ambientStrength; // 0–1, фон
uniform float specularStrength; // 0–1, интенсивность блика

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

void main() {
    vec3 albedo = texture(albedoMap, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 color = vec3(0.0);

    const float MAX_AMBIENT = 1.0;    // Максимальная ambient освещенность
    const float MAX_DIFFUSE = 32.0;    // Максимальная diffuse освещенность  
    const float MAX_SPECULAR = 32.0;   // Максимальная specular интенсивность

    // коэффициент Френеля для диэлектрика
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    for (int i = 0; i < numPointLights; ++i) {
        PointLight light = pointLights[i];
        vec3 lightPos = light.position.xyz;
        vec3 lightColor = light.color.rgb;
        float lightIntensity = light.color.a;

        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float distance = length(lightPos - fs_in.FragPos);
        float attenuation = lightIntensity / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        float diff = max(dot(normal, lightDir), 0.0);

        vec3 ambient = ambientStrength * MAX_AMBIENT * lightColor;
        vec3 diffuse = (1.0 - metallic) * diff * MAX_DIFFUSE * lightColor;

        if (diff > 0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float shininess = mix(128.0, 4.0, roughness);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = specularStrength * spec * MAX_SPECULAR * lightColor;
            // specular *= mix(vec3(1.0), F0 * 5.0, metallic);

            if (hasSpecularMap) {
                specular *= texture(specularMap, fs_in.TexCoord).rgb;
            }

            color += specular;
        }

        color += diffuse;
        color *= attenuation;
        color += ambient;
    }

    FragColor = vec4(color * albedo, 1.0);
}
