#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D TextureSampler;

void main()
{
    vec4 texColor = texture(TextureSampler, TexCoord);
    if (texColor.a < 0.1) discard;
    FragColor = texColor;
}
