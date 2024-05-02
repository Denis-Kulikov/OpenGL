#version 330

out vec4 FragColor;

void main()
{
    FragColor = vec4(gColor.x, gColor.y, gColor.z, 0.0f);
}


#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 gColor;

void main() {
    vec4 texColor = texture(textureSampler, TexCoord);
    if (texColor.a < 0.1) discard;
    FragColor = texColor;
}
