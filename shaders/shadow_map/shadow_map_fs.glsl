#version 330 core
in vec4 FragPosWorld;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float lightDistance = length(FragPosWorld.xyz - lightPos);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}
