#include <render/pipeline.hpp>

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
