#ifndef  _DATATYPES_H_
#define  _DATATYPES_H_

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

struct PersProjInfo {
  float FOV;
  float Width; 
  float Height;
  float zNear;
  float zFar;
};

struct Vector3f {
  float x;
  float y;
  float z;

  Vector3f() {}
  
  Vector3f(float _x, float _y, float _z) :
      x(_x), y(_y), z(_z) {}

  Vector3f cross(const Vector3f& v) {
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
  }

  Vector3f& normalize() {
    const float length = sqrtf(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    return *this;
  }

  Vector3f& operator*= (float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  Vector3f operator* (float f) const {
    return Vector3f(x * f, y * f, z * f);
  }

  Vector3f& operator+= (const Vector3f& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector3f& operator-= (const Vector3f& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
};

struct Vector4f {
  float x;
  float y;
  float z;
  float w;

  Vector4f() {}
  
  Vector4f(float _x, float _y, float _z, float _w) :
      x(_x), y(_y), z(_z), w(_w) {}
};

struct Matrix4f {
  float m[4][4];

  void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
  }
  
  void InitRotateTransform(float RotateX, float RotateY, float RotateZ) {
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
  
  void InitTranslationTransform(float x, float y, float z) {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  void InitPerspectiveProj(const PersProjInfo& persProj) {
    const float ar = persProj.Width / persProj.Height;
    const float zNear = persProj.zNear;
    const float zFar = persProj.zFar;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(persProj.FOV / 2.0));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0f;
  }

  void InitCameraTransform(const Vector3f& Target, const Vector3f& Up) {
    Vector3f N = Target;
    N.normalize();
    Vector3f U = Up;
    U.normalize();
    U = U.cross(Target);
    Vector3f V = N.cross(U);

    m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
    m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
    m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }

  Matrix4f operator* (const Matrix4f& Right) const {
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
};


#endif   // _DATATYPES_H_
