#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec2 TexCoord;

// uniform mat4 gBones[128];
uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

const int MAX_BONES = 128; 
uniform vec4 gDQ[MAX_BONES * 2];

vec4 quat_mul(vec4 q1, vec4 q2) {
    return vec4(
        q1.w * q2.xyz + q2.w * q1.xyz + cross(q1.xyz, q2.xyz),
        q1.w * q2.w - dot(q1.xyz, q2.xyz)
    );
}

// Смешивание dual quaternion'ов
void dual_quat_blend(
    in ivec4 boneIDs,
    in vec4 weights,
    out vec4 blend_real,
    out vec4 blend_dual
) {
    blend_real = vec4(0.0);
    blend_dual = vec4(0.0);

    for (int i = 0; i < 4; ++i) {
        int id = boneIDs[i];
        float w = weights[i];

        vec4 qr = gDQ[id * 2];
        // Убедимся, что смешиваем в одном полушарии
        if (dot(blend_real, qr) < 0.0)
            qr = -qr;

        vec4 qd = gDQ[id * 2 + 1];

        blend_real += qr * w;
        blend_dual += qd * w;
    }

    // Нормализация
    float norm = length(blend_real);
    blend_real /= norm;
    blend_dual /= norm;
}

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
    vec4 blend_real, blend_dual;
    dual_quat_blend(aBoneIDs, aWeights, blend_real, blend_dual);

    vec3 skinnedPos = transform_position(aPosition, blend_real, blend_dual);

    gl_Position = Projection * View * Model * vec4(skinnedPos, 1.0);
    TexCoord = aTexCoords;
}
