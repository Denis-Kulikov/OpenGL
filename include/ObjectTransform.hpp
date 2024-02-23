#include "math_3d.hpp"

class objectTransform 
{
public:
    objectTransform() {
        Scale       = Vector3<GLfloat>(1.0f, 1.0f, 1.0f);
        WorldPos    = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
        Rotate      = Vector3<GLfloat>(0.0f, 0.0f, 0.0f);
    }

    Vector3<GLfloat> GetScale();
    Vector3<GLfloat> GetWorldPos();
    Vector3<GLfloat> GetRotate();


    void SetScale(GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ);
    void SetScale(Vector3<GLfloat> v_Scale);

    void SetWorldPos(GLfloat WorldPosX, GLfloat WorldPosY, GLfloat WorldPosZ);
    void SetWorldPos(Vector3<GLfloat> v_WorldPos);

    void SetRotate(GLfloat RotateX, GLfloat RotateY, GLfloat RotateZ);
    void SetRotate(Vector3<GLfloat> v_Rotate);

    void SetTransform(objectTransform trans);

    Vector3<GLfloat> Scale;
    Vector3<GLfloat> WorldPos;
    Vector3<GLfloat> Rotate;
};
