#include <render/pipeline.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Pipeline::Pipeline() {}

void Pipeline::drawSkybox(Actor &skybox) {
    if (camera == nullptr) return;
    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    GLboolean oldDepthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthMask);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skybox.rootComponent->SetPosition(camera->GetGlobalPosition());

    glm::quat quaternion = glm::quat(glm::radians(glm::vec3(0, 0, 180)));
    if (skybox.rootComponent->GetRotation() != quaternion)
        skybox.rootComponent->SetRotation(quaternion);

    if (skybox.rootComponent->GetScale() != glm::vec3(2))
        skybox.rootComponent->SetScale(glm::vec3(2));
        
    skybox.Render();

    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFunc);
    glDepthMask(oldDepthMask);
}

glm::mat4 Pipeline::GetModel(const Transform& transform) const
{
    glm::mat4 scaleMatrix = glm::scale(transform.GetScale());
    glm::mat4 rotateMatrix = glm::mat4_cast(transform.GetRotation());
    glm::mat4 translationMatrix = glm::translate(transform.GetPosition());

    return translationMatrix * rotateMatrix * scaleMatrix;
}

void Pipeline::UpdatePV() {
    PV = ProjTrans * View;
}

void Pipeline::UpdateProj(bool perspective) {
    if (perspective)
        UpdatePerspective();
    else
        UpdateOrthographic();
}

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

void Pipeline::UpdateView() {
    if (camera == nullptr) return;
    const auto& c = camera->camera;

    #if false
        View = glm::lookAt(
            camera->GetGlobalPosition(),
            camera->GetGlobalPosition() + c.Params.Target,
            c.Params.Up
        );
    #else
        View = LookAtLH(
            camera->GetGlobalPosition(),                      // eye (позиция камеры)
            camera->GetGlobalPosition() + c.Params.Target,    // center (точка, куда смотрит камера)
            c.Params.Up 
        );
    #endif
}

void Pipeline::UpdatePerspective() {
    if (camera == nullptr) return;
    const auto& c = camera->camera;

    ProjTrans = PerspectiveLH(
        glm::radians(c.PersProj.FOV),
        static_cast<float>(c.PersProj.Width) / c.PersProj.Height,
        c.PersProj.zNear,
        c.PersProj.zFar
    );

    PV = ProjTrans * View;
}

void Pipeline::UpdateOrthographic() {
    if (camera == nullptr) return;
    const auto& c = camera->camera;

    float halfW = c.PersProj.Width / 2.0f;
    float halfH = c.PersProj.Height / 2.0f;

    ProjTrans = glm::ortho(
        -halfW, halfW,
        -halfH, halfH,
        c.PersProj.zNear, c.PersProj.zFar
    );

    PV = ProjTrans * View;
}
