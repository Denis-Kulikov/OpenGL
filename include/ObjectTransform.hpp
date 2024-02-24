#include "math_3d.hpp"

class objectTransform 
{
public:
    objectTransform() {
        Scale       = Vector3<GLfloat>(1.0f, 1.0f, 1.0f);
        WorldPos    = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
        Rotate      = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
    }

    Vector3<GLfloat> GetScale() const;
    Vector3<GLfloat> GetWorldPos() const;
    Vector3<GLfloat> GetRotate() const;


    void SetScale(const GLfloat &ScaleX, const GLfloat &ScaleY, const GLfloat &ScaleZ);
    void SetScale(const Vector3<GLfloat> &v_Scale);

    void SetWorldPos(const GLfloat &WorldPosX, const GLfloat &WorldPosY, const GLfloat &WorldPosZ);
    void SetWorldPos(const Vector3<GLfloat> &v_WorldPos);

    void SetRotate(const GLfloat &RotateX, const GLfloat &RotateY, const GLfloat &RotateZ);
    void SetRotate(const Vector3<GLfloat> &v_Rotate);

    void SetTransform(const objectTransform &trans);

    Vector3<GLfloat> Scale;
    Vector3<GLfloat> WorldPos;
    Vector3<GLfloat> Rotate;
};
