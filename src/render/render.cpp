#include <render/render.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Render::Render() {}

void Render::drawSkybox(Actor &skybox) {
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
    skybox.rootComponent->SetRotation(glm::vec3(0, 0, 180));
    skybox.rootComponent->SetScale(glm::vec3(2));
    skybox.Render();

    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFunc);
    glDepthMask(oldDepthMask);
}

glm::mat4 Render::GetModel(const Transform& transform) const
{
    glm::mat4 scaleMatrix = glm::scale(transform.GetScale());
    glm::mat4 rotateMatrix = glm::mat4_cast(transform.GetRotation());
    glm::mat4 translationMatrix = glm::translate(transform.GetPosition());

    return translationMatrix * rotateMatrix * scaleMatrix;
}

glm::mat4 Render::GetPV() {
    return PV;
}

void Render::UpdatePV(bool perspective) {
    if (perspective)
        UpdatePV_Perspective();
    else
        UpdatePV_Orthographic();
}

void Render::UpdatePV_Perspective() {
    if (camera == nullptr) return;
    const auto& c = camera->camera;

    View = glm::lookAt(
        camera->GetGlobalPosition(),
        camera->GetGlobalPosition() + c.Params.Target,
        c.Params.Up
    );

    ProjTrans = glm::perspective(
        glm::radians(c.PersProj.FOV),
        static_cast<float>(c.PersProj.Width) / c.PersProj.Height,
        c.PersProj.zNear,
        c.PersProj.zFar
    );

    PV = ProjTrans * View;
}

void Render::UpdatePV_Orthographic() {
    if (camera == nullptr) return;
    const auto& c = camera->camera;

    View = glm::lookAt(
        camera->GetGlobalPosition(),
        camera->GetGlobalPosition() + c.Params.Target,
        c.Params.Up
    );

    float halfW = c.PersProj.Width / 2.0f;
    float halfH = c.PersProj.Height / 2.0f;

    ProjTrans = glm::ortho(
        -halfW, halfW,
        -halfH, halfH,
        c.PersProj.zNear, c.PersProj.zFar
    );

    PV = ProjTrans * View;
}
