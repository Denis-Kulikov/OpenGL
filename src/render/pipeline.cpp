#include <render/pipeline.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Matrix4f Pipeline::GetTransform(const objectTransform& transform) const
{
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjTrans;
    glm::vec3 CameraPos = -camera.GetPosition();

    ScaleTrans.InitScaleTransform(transform.Scale.x, transform.Scale.y, transform.Scale.z);
    RotateTrans.InitRotateTransform(transform.Rotate.x, transform.Rotate.y, transform.Rotate.z);
    TranslationTrans.InitTranslationTransform(transform.WorldPos.x, transform.WorldPos.y, transform.WorldPos.z);
    CameraTranslationTrans.InitTranslationTransform(CameraPos.x, CameraPos.y, CameraPos.z);
    CameraRotateTrans.InitCameraTransform(camera.Params.Target, camera.Params.Up);
    PersProjTrans.InitPersProjTransform(camera.PersProj.FOV, camera.PersProj.Width, camera.PersProj.Height, camera.PersProj.zNear, camera.PersProj.zFar);

    return PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
}

Matrix4f Pipeline::GetWorld(const objectTransform& transform) const
{
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

    ScaleTrans.InitScaleTransform(transform.Scale.x, transform.Scale.y, transform.Scale.z);
    RotateTrans.InitRotateTransform(transform.Rotate.x, transform.Rotate.y, transform.Rotate.z);
    TranslationTrans.InitTranslationTransform(transform.WorldPos.x, transform.WorldPos.y, transform.WorldPos.z);

    return TranslationTrans * RotateTrans * ScaleTrans;
}

glm::mat4 Pipeline::GetModel(const objectTransform& transform) const
{
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(transform.Scale.x, transform.Scale.y, transform.Scale.z));

    glm::mat4 rotateX = glm::rotate(glm::radians(transform.Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::radians(transform.Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateZ = glm::rotate(glm::radians(transform.Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotateMatrix = rotateZ * rotateY * rotateX;

    glm::mat4 translationMatrix = glm::translate(glm::vec3(transform.WorldPos.x, transform.WorldPos.y, transform.WorldPos.z));

    return translationMatrix * rotateMatrix * scaleMatrix;
}
