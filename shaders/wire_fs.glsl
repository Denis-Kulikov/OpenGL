#version 330

out vec4 FragColor;
uniform vec3 gColor;

void main()
{
    FragColor = vec4(gColor.x, gColor.y, gColor.z, 0.0f);
}
