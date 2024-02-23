#ifndef MATH_3D_H
#define	MATH_3D_H

#include "lib.hpp"

# define M_PI           3.14159265358979323846

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

float radians(float degrees);

template<typename T>
class Vector3
{
public:
    T x;
    T y;
    T z;

    Vector3() {}

    Vector3(T _x, T _y, T _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3<T>& Normalize();
    T VDot(const Vector3<T> &v);
    T VLenSq();
    T VLen();
    T VCSum();
    T Distance(const Vector3<T> &v);
    Vector3<T> operator+(const Vector3<T> &v);
    Vector3<T> operator+(const T s);
    Vector3<T> operator-(const Vector3<T> &v);
    Vector3<T> operator-(const T s);
    Vector3<T> operator*(const Vector3<T> &v);
    Vector3<T> operator*(const T s);
    Vector3<T> operator/(const Vector3<T> &v);
    Vector3<T> operator/(const T s);
    void VSet(const Vector3<T> &v);
    void VSet(T s);
    void VSet(T sx, T sy, T sz) {
        x = sx;
        y = sy;
        z = sz;
    };

    void VZero();
    Vector3<T> VVV();

    Vector3 Cross(const Vector3& v) const;

    void Print() const;
};

class Matrix4f
{
public:
    float m[4][4];

    Matrix4f() {}

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
    void InitCameraTransform(const Vector3<float>& Target, const Vector3<float>& Up);
    void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);
};

#endif	/* MATH_3D_H */
