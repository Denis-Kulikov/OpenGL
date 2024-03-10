#pragma once

#include "../include/lib-project/lib.hpp"

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

    Vector3<T>& Normalize() {
        const T Length = sqrtf(x * x + y * y + z * z);

        x /= Length;
        y /= Length;
        z /= Length;

        return *this;
    }

    T VDot(const Vector3<T> &v) {
        return x * v.x + y * v.y + z * v.z;
    }

    T VLenSq() {
        return VDot(*this);
    }

    T VLen() {
        return sqrt(VLenSq());
    }

    T VCSum() {
        return x + y + z;
    }


    T Distance(const Vector3<T> &v) const {
        return sqrt(pow(v.x - x, 2) + pow(v.y - y, 2) + pow(v.z - z, 2));
    }

    Vector3<T> operator+(const Vector3<T> &v) {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T> operator+(const T s) {
        return Vector3(x, y, z) + Vector3(s, s, s);
    }

    Vector3<T> operator-(const Vector3<T> &v) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3<T> operator-(const T s) {
        return Vector3(x, y, z) - Vector3(s, s, s);
    }

    Vector3<T> operator*(const Vector3<T> &v) {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3<T> operator*(const T s) {
        return Vector3(x, y, z) * Vector3(s, s, s);
    }

    Vector3<T> operator/(const Vector3<T> &v) {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }

    Vector3<T> operator/(const T s) {
        return Vector3(x, y, z) / Vector3(s, s, s);
    }


    Vector3<T>& operator*=(const Vector3<T> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vector3<T>& operator*=(const T &s) {
        Vector3<T> temp(s, s, s);
        *this *= temp;
        return *this;
    }


    Vector3<T>& operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3<T>& operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3<T>& operator/=(const Vector3<T> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }



    void VSet(const Vector3<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    void VSet(T s) {
        x = s;
        y = s;
        z = s;
    }

    void VSet(T sx, T sy, T sz) {
        x = sx;
        y = sy;
        z = sz;
    }

    void VZero() {
        VSet(0);
    }

    Vector3<T> VVV() {
        return Vector3(0, 0, 0);
    }

    Vector3 Cross(const Vector3& v) const {
        const T _x = y * v.z - z * v.y;
        const T _y = z * v.x - x * v.z;
        const T _z = x * v.y - y * v.x;

        return Vector3(_x, _y, _z);
    }
};

