#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureSampler;

void main() {
    vec4 texColor = texture(textureSampler, TexCoord);
    if (texColor.a < 0.1) discard;
    FragColor = texColor;
}
