#ifndef MATH_3D_H
#define	MATH_3D_H

#include <stdio.h>
#include <cmath>

# define M_PI           3.14159265358979323846

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

float radians(float degrees);

struct Vector2f
{
    float x;
    float y;

    Vector2f()
    {
    }

    Vector2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};

class Vector3i
{
public:
    int x, y, z;
    Vector3i() {};
    Vector3i(int _x, int _y, int _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3i VMul(Vector3i v) {return Vector3i(x * v.x, y * v.y, z * v.z);};
    Vector3i VScale(float s) {return VMul(Vector3i(s, s, s));};
    int* GetData() { return &x; }
};

class Vector3f
{
public:
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f& Normalize();
    float VDot(const Vector3f &v);
    float VLenSq();
    float VLen();
    float VCSum();
    float Distance(const Vector3f &v);
    Vector3f VAdd(const Vector3f &v);
    Vector3f VSub(const Vector3f &v);
    Vector3f VMul(const Vector3f &v);
    Vector3f VDiv(const Vector3f &v);
    Vector3f VDiv(const Vector3i &v);
    Vector3f VScale(float s);
    void VSet(const Vector3f &v);
    void VSet(float s);
    void VSet(float sx, float sy, float sz);
    void VSetI(const Vector3i &v);
    void VZero();
    Vector3f VVV();

    Vector3f Cross(const Vector3f& v) const;

    void Print() const;
};

class Matrix4f
{
public:
    float m[4][4];

    Matrix4f()
    {        
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
    void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);
};


#endif	/* MATH_3D_H */
