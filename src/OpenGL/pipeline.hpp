#include "math_3d.h"

class pipeline
{
public:
    pipeline() {
        m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
        m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
        m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void InitScaleTransform (Matrix4f& m) const;
    void InitRotateTransform (Matrix4f& m) const;
    void InitTranslationTransform (Matrix4f& m) const;
    const Matrix4f *GetTransform ();

    void Scale(float x, float y, float z) {m_scale.x = x; m_scale.y = y; m_scale.z = z;};
    void WorldPos(float x, float y, float z) {m_worldPos.x = x; m_worldPos.y = y; m_worldPos.z = z;};
    void Rotate(float x, float y, float z) {m_rotateInfo.x = x; m_rotateInfo.y = y; m_rotateInfo.z = z;};

    double WorldPosZ() {return m_worldPos.z;};

private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    Matrix4f m_transformation;
};
