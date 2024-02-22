#include "../include/pipeline.hpp"

const Matrix4f* Pipeline::GetTrans()
{
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    ScaleTrans.InitScaleTransform(object.Scale.x, object.Scale.y, object.Scale.z);
    RotateTrans.InitRotateTransform(object.Rotate.x, object.Rotate.y, object.Rotate.z);
    TranslationTrans.InitTranslationTransform(object.WorldPos.x, object.WorldPos.y, object.WorldPos.z);
    CameraTranslationTrans.InitTranslationTransform(-camera.Params.WorldPos.x, -camera.Params.WorldPos.y, -camera.Params.WorldPos.z);
    CameraRotateTrans.InitCameraTransform(camera.Params.Target, camera.Params.Up);
    PersProjTrans.InitPersProjTransform(camera.PersProj.FOV, camera.PersProj.Width, camera.PersProj.Height, camera.PersProj.zNear, camera.PersProj.zFar);

    m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &m_transformation;
}