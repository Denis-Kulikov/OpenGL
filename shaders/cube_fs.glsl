#version 330 core

out vec4 FragColor;

in vec2 TexCoordOut;

uniform sampler2D textureSampler;

void main() {
    vec2 coord = TexCoordOut;
    float seamFix = 0.002;
    coord.x = clamp(coord.x, seamFix, 1.0 - seamFix);
    coord.y = clamp(coord.y, seamFix, 1.0 - seamFix);
    
    vec4 texColor = texture(textureSampler, coord);
    if (texColor.a < 0.1) discard;
    FragColor = texColor;
}
