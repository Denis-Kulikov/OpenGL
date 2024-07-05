#version 330 core

    // glUniform3fv(_sprite->gObjectLocation, 3, reinterpret_cast<const GLfloat*>(pipeline.object));
    // glUniform3fv(_sprite->gCameraParamsLocation, 3, reinterpret_cast<const GLfloat*>(&pipeline.camera->Params));
    // glUniform1fv(_sprite->gPersProjParamsLocation, 5, reinterpret_cast<const GLfloat*>(&pipeline.camera->PersProj));


#define TRANSLATION 0
#define ROTATION 1
#define TARGET 1
#define SCALE 2
#define UP 2

uniform mat4 gWorld;

uniform vec3 object[3];
uniform vec3 cameraParams[3];
uniform float PersProjParams[5];

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

mat4 InitIdentity() {
    return mat4(1.0);
}

mat4 InitScaleTransform(vec3 Scale) {
    mat4 m = mat4(1.0);
    m[0][0] = Scale.x;
    m[1][1] = Scale.y;
    m[2][2] = Scale.z;
    return m;
}

mat4 InitRotateTransform(vec3 Rotate) {
    float x = radians(Rotate.x);
    float y = radians(Rotate.y);
    float z = radians(Rotate.z);

    mat4 rx = mat4(1.0);
    mat4 ry = mat4(1.0);
    mat4 rz = mat4(1.0);

    rx[1][1] = cos(x);
    rx[1][2] = -sin(x);
    rx[2][1] = sin(x);
    rx[2][2] = cos(x);

    ry[0][0] = cos(y);
    ry[0][2] = -sin(y);
    ry[2][0] = sin(y);
    ry[2][2] = cos(y);

    rz[0][0] = cos(z);
    rz[0][1] = -sin(z);
    rz[1][0] = sin(z);
    rz[1][1] = cos(z);

    return rz * ry * rx;
}

mat4 InitTranslationTransform(vec3 Translation) {
    mat4 m = mat4(1.0);
    m[3][0] = Translation.x;
    m[3][1] = Translation.y;
    m[3][2] = Translation.z;
    return m;
}

mat4 InitCameraTransform(vec3 Target, vec3 Up) {
    vec3 N = normalize(Target);
    vec3 U = normalize(cross(Up, N));
    vec3 V = cross(N, U);

    mat4 m = mat4(1.0);
    m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z;
    m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z;
    m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z;

    return m;
}

mat4 InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar) {
    float ar = Width / Height;
    float zRange = zNear - zFar;
    float tanHalfFOV = tan(radians(FOV / 2.0));

    mat4 m = mat4(0.0);
    m[0][0] = 1.0 / (tanHalfFOV * ar);
    m[1][1] = 1.0 / tanHalfFOV;
    m[2][2] = (zNear + zFar) / zRange;
    m[2][3] = -1.0;
    m[3][2] = (2.0 * zFar * zNear) / zRange;

    return m;
}

void main()
{
    mat4 ScaleTrans = InitScaleTransform(-object[SCALE]);
    mat4 RotateTrans = InitRotateTransform(-object[ROTATION]);
    mat4 TranslationTrans = InitTranslationTransform(-object[TRANSLATION]);

    mat4 CameraTranslationTrans = InitTranslationTransform(cameraParams[TRANSLATION]);
    mat4 CameraRotateTrans = InitCameraTransform(cameraParams[TARGET], -cameraParams[UP]);
    mat4 PersProjTrans = InitPersProjTransform(PersProjParams[0], PersProjParams[1], PersProjParams[2], PersProjParams[3], PersProjParams[4]);

    mat4 m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;

    gl_Position = m_transformation * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
}
