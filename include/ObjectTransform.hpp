#include "math_3d.hpp"

class ObjectTransform 
{
public:
    ObjectTransform()
    {
        Scale = Vector3f(1.0f, 1.0f, 1.0f);
        WorldPos = Vector3f(0.0f, 0.0f, 0.0f);
        Rotate = Vector3f(0.0f, 0.0f, 0.0f);
    }

    Vector3f GetScale();
    Vector3f GetWorldPos();
    Vector3f GetRotate();


    void SetScale(double ScaleX, double ScaleY, double ScaleZ);
    void SetScale(Vector3f v_Scale);

    void SetWorldPos(double WorldPosX, double WorldPosY, double WorldPosZ);
    void SetWorldPos(Vector3f v_WorldPos);

    void SetRotate(double RotateX, double RotateY, double RotateZ);
    void SetRotate(Vector3f v_Rotate);

    void SetTransform(ObjectTransform trans);

    Vector3f Scale;
    Vector3f WorldPos;
    Vector3f Rotate;
};
