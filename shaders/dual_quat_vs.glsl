#version 330 core

uniform mat4 PV;
uniform vec4 gDQ[2];

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoordOut;

vec3 rotate_vector(vec3 v, vec4 q) {
    // v' = q * v * q⁻¹
    vec3 u = q.xyz;
    float s = q.w;
    return 2.0 * dot(u, v) * u + (s*s - dot(u, u)) * v + 2.0 * s * cross(u, v);
}

vec3 transform_position(vec3 pos, vec4 qr, vec4 qd) {
    // Вращаем позицию
    vec3 rotated = rotate_vector(pos, qr);

    // Извлекаем трансляцию: t = 2 * (qd * conj(qr)).xyz
    vec3 t = 2.0 * (qd.xyz * qr.w - qr.xyz * qd.w + cross(qr.xyz, qd.xyz));

    return rotated + t;
}


void main()
{
    vec3 transformed = transform_position(aPosition, gDQ[0], gDQ[1]);
    gl_Position = PV * vec4(transformed, 1.0);
    TexCoordOut = aTexCoord;
}