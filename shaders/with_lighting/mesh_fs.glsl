#version 330 core

layout (std140) uniform Matrices {
    mat4 Projection;
    mat4 View;
    vec3 viewPos;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

struct PointLight {
    vec4 position;
    vec4 color;
    float radius;
    float padding1;
    float padding2;
    float padding3;
};

layout(std430, binding = 1) buffer Lights {
    int count;
    PointLight lights[];
};

uniform sampler2D diffuseMap;

out vec4 FragColor;

void main() {
    vec3 albedo = texture(diffuseMap, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < count; ++i) {
        PointLight light = lights[i];
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

        result += (ambient + diffuse + specular) * attenuation;
    }

    FragColor = vec4(result * albedo, 1.0);

}
