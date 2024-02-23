#include <glfw.hpp>

float radians(float degrees) {
    return degrees * M_PI / 180.0;
}

template<typename T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
    const T _x = y * v.z - z * v.y;
    const T _y = z * v.x - x * v.z;
    const T _z = x * v.y - y * v.x;

    return Vector3(_x, _y, _z);
}

template<typename T>
Vector3<T>& Vector3<T>::Normalize()
{
    const T Length = sqrtf(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
}

template<typename T>
T Vector3<T>::VDot(const Vector3<T> &v)
{
    return x * v.x + y * v.y + z * v.z;
}

template<typename T>
T Vector3<T>::VLenSq()
{
    return VDot(*this);
}

template<typename T>
T Vector3<T>::VLen()
{
    return sqrt(VLenSq());
}

template<typename T>
T Vector3<T>::VCSum()
{
    return x + y + z;
}

template<typename T>
T Vector3<T>::Distance(const Vector3<T> &v)
{
    return sqrt(pow(v.x - x, 2) + pow(v.y - y, 2) + pow(v.z - z, 2));
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &v)
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const T s)
{
    return Vector3(x, y, z) + Vector3(s, s, s);
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &v)
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const T s)
{
    return Vector3(x, y, z) - Vector3(s, s, s);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const Vector3<T> &v)
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const T s)
{
    return Vector3(x, y, z) * Vector3(s, s, s);
}

template<typename T>
Vector3<T> Vector3<T>::operator/(const Vector3<T> &v)
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

// template<typename T>
// Vector3<T> Vector3<T>::operator/(const Vector3i &v)
// {
//     return Vector3(x / v.x, y / v.y, z / v.z);
// }

template<typename T>
Vector3<T> Vector3<T>::operator/(const T s)
{
    return Vector3(x, y, z) / Vector3(s, s, s);
}


template<typename T>
void Vector3<T>::VSet(const Vector3<T> &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

template<typename T>
void Vector3<T>::VSet(T s)
{
    x = s;
    y = s;
    z = s;
}

// void Vector3<float>::VSet(float sx, float sy, float sz)
// {
//     x = sx;
//     y = sy;
//     z = sz;
// }

// void Vector3::VSetI(const Vector3i &v)
// {
//     x = v.x;
//     y = v.y;
//     z = v.z;
// }

template<typename T>
void Vector3<T>::VZero()
{
    VSet(0);
}

template<typename T>
Vector3<T> Vector3<T>::VVV()
{
    return Vector3(0, 0, 0);
}

template<typename T>
T VectorDot(Vector3<T> a, Vector3<T> b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// template<typename T>
// Vector3<T> Vector3<T>::VectorNormalize(Vector3<T> v)
// {
//     T len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
//     if (len > 0.0f) {
//         T invLen = 1.0f / len;
//         v.x *= invLen;
//         v.y *= invLen;
//         v.z *= invLen;
//     }
//     return v;
// }

// template<typename T>
// Vector3<T> VectorSubtract(Vector3<T> a, Vector3<T> b)
// {
//     Vector3<T> result;
//     result.x = a.x - b.x;
//     result.y = a.y - b.y;
//     result.z = a.z - b.z;
//     return result;
// }

// template<typename T>
// Vector3<T> VectorAdd(Vector3<T> a, Vector3<T> b)
// {
//     Vector3<T> result;
//     result.x = a.x + b.x;
//     result.y = a.y + b.y;
//     result.z = a.z + b.z;
//     return result;
// }

template<typename T>
Vector3<T> VectorScale(const Vector3<T> &v, T s)
{
    Vector3<T> result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

template<typename T>
void Vector3<T>::Print() const
{
    printf("%.02f, %.02f, %.02f\n", x, y, z);
}


void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
    Matrix4f rx, ry, rz;

    const float x = ToRadian(RotateX);
    const float y = ToRadian(RotateY);
    const float z = ToRadian(RotateZ);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    *this = rz * ry * rx;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


void Matrix4f::InitCameraTransform(const Vector3<float>& Target, const Vector3<float>& Up)
{
    Vector3<float> N = Target;
    N.Normalize();
    Vector3<float> U = Up;
    U.Normalize();
    U = U.Cross(N);
    Vector3<float> V = N.Cross(U);
       
    m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
    m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
    m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;        
}

void Matrix4f::InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar)
{
    const float ar         = Width / Height;
    const float zRange     = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

    m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear -zFar)/zRange ; m[2][3] = 2.0f * zFar*zNear/zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
}
