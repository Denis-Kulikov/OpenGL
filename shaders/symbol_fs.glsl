#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 gColor;

void main() {
    vec4 sampled = vec4(texture(textureSampler, TexCoord).r);
    if (sampled.r < 0.1) discard;
    FragColor = vec4(gColor.x, gColor.y, gColor.z, sampled.r);
}
