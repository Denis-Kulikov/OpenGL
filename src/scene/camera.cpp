#include <scene/camera.hpp>

glm::mat4 LookAtLH(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    glm::vec3 F = glm::normalize(center - eye);
    glm::vec3 R = glm::normalize(glm::cross(F, up));  // ← обратный порядок!
    glm::vec3 U = glm::cross(R, F);

    glm::mat4 result(1.0f);
    result[0][0] = R.x; result[1][0] = R.y; result[2][0] = R.z;
    result[0][1] = U.x; result[1][1] = U.y; result[2][1] = U.z;
    result[0][2] =-F.x; result[1][2] =-F.y; result[2][2] =-F.z;
    result[3][0] = -glm::dot(R, eye);
    result[3][1] = -glm::dot(U, eye);
    result[3][2] =  glm::dot(F, eye); // обратный знак

    return result;
}
glm::mat4 PerspectiveLH(float fovY, float aspect, float near, float far) {
    float f = 1.0f / tan(fovY / 2.0f);
    glm::mat4 result(0.0f);

    result[0][0] = f / aspect;
    result[1][1] = f;
    result[2][2] = far / (far - near);
    result[2][3] = 1.0f;
    result[3][2] = -(near * far) / (far - near);

    return result;
}
glm::mat4 OrthoLH(float left, float right, float bottom, float top, float near, float far)
{
    glm::mat4 result(1.0f);

    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = 1.0f / (far - near); // отличие от RH: нет инверсии оси Z

    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(top + bottom) / (top - bottom);
    result[3][2] = -near / (far - near); // отличие от RH

    return result;
}

Camera::Camera(const Camera &camera) 
{
    Params.Target   = camera.Params.Target;
    Params.Up       = camera.Params.Up;

    PersProj.FOV    = camera.PersProj.FOV;
    PersProj.Width  = camera.PersProj.Width;
    PersProj.Height = camera.PersProj.Height;
    PersProj.zNear  = camera.PersProj.zNear;
    PersProj.zFar   = camera.PersProj.zFar;
}
Camera::Camera()
{
    Params.Target      = glm::vec3(0.0f, 0.0f, -1.0f);
    Params.Up          = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::SetPerspectiveProj(GLfloat FOV, GLfloat Width, GLfloat Height, GLfloat zNear, GLfloat zFar)
{
    PersProj.FOV    = FOV;
    PersProj.Width  = Width;
    PersProj.Height = Height;
    PersProj.zNear  = zNear;
    PersProj.zFar   = zFar;
}

void Camera::SetPerspectiveProj(const Camera &_camera)
{
    PersProj.FOV    = _camera.PersProj.FOV;
    PersProj.Width  = _camera.PersProj.Width;
    PersProj.Height = _camera.PersProj.Height;
    PersProj.zNear  = _camera.PersProj.zNear;
    PersProj.zFar   = _camera.PersProj.zFar;
}

void Camera::SetCamera(const glm::vec3& Target, const glm::vec3& Up)
{
    Params.Target   = Target;
    Params.Up       = Up;
}

void Camera::SetCamera(const Camera &_camera)
{
    SetCamera(_camera.Params.Target, _camera.Params.Up);
    SetPerspectiveProj(_camera.PersProj.FOV, _camera.PersProj.Width, _camera.PersProj.Height, _camera.PersProj.zNear, _camera.PersProj.zFar);
}

void Camera::UpdateTarget() {
    const float pitch_limit = 89.0f;
    float yaw = glm::radians(GetYaw());
    float pitch = glm::radians(glm::clamp(GetPitch(), -pitch_limit, pitch_limit));

    glm::vec3 front;
    front.x = cos(pitch) * sin(yaw);
    front.y = sin(pitch);
    front.z = cos(pitch) * cos(yaw);
    Params.Target = glm::normalize(front);
}
