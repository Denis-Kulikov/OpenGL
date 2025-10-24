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

    PointLight pointLights[128];
    DirectionalLight dirLights[16];
    // SpotLight spotLights[32];
};

uniform sampler2D albedoMap;

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

    for (int i = 0; i < numPointLights; ++i) {
        PointLight light = pointLights[i];
        vec3 lightPos = light.position.xyz;
        vec3 lightColor = light.color.rgb;

        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float distance = length(lightPos - fs_in.FragPos);
        float attenuation = clamp(1.0 - distance / light.radius, 0.0, 1.0);

        float diff = max(dot(normal, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

        vec3 ambient = 0.05 * lightColor;
        vec3 diffuse = diff * lightColor;
        vec3 specular = spec * lightColor;

        color += (ambient + diffuse + specular) * attenuation;
    }

    for (int i = 0; i < numDirectionalLights; ++i) {
        DirectionalLight light = dirLights[i];
        vec3 lightDir = normalize(-light.direction.xyz); // Направление К источнику
        vec3 lightColor = light.color.rgb;
        float intensity = light.direction.w;

        // Диффузная составляющая
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * intensity;

        // Спекулярная составляющая
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lightColor * intensity;

        color += diffuse + specular;
    }

    FragColor = vec4(color * albedo, 1.0);
}
