#include "OpenGL/dataTypes.h"
#include <iostream>

Quaternion Quaternion::conjugate() {
  Quaternion ret(-x, -y, -z, w);
  return ret;
}

Quaternion& Quaternion::normalize() {
  float length = sqrtf(x * x + y * y + z * z + w * w);

  x /= length;
  y /= length;
  z /= length;
  w /= length;
  return *this;
}

Quaternion Quaternion::operator* (const Quaternion& r) {
  const float w_ = w * r.w - x * r.x - y * r.y - z * r.z;
  const float x_ = x * r.w + w * r.x + y * r.z - z * r.y;
  const float y_ = y * r.w + w * r.y + z * r.x - x * r.z;
  const float z_ = z * r.w + w * r.z + x * r.y - y * r.x;

  return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::operator* (const Vector3f& v) {
  const float w_ = - (x * v.x) - (y * v.y) - (z * v.z);
  const float x_ =   (w * v.x) + (y * v.z) - (z * v.y);
  const float y_ =   (w * v.y) + (z * v.x) - (x * v.z);
  const float z_ =   (w * v.z) + (x * v.y) - (y * v.x);

  return Quaternion(x_, y_, z_, w_);
}

void Vector3f::rotate(float Angle, const Vector3f& Axe) {
  const float SinHalfAngle = sinf(ToRadian(Angle/2.0f));
  const float CosHalfAngle = cosf(ToRadian(Angle/2.0f));

  const float Rx = Axe.x * SinHalfAngle;
  const float Ry = Axe.y * SinHalfAngle;
  const float Rz = Axe.z * SinHalfAngle;
  const float Rw = CosHalfAngle;
  Quaternion RotationQ(Rx, Ry, Rz, Rw);
  // std::cout << "Before rotate (" << Rx << ", " << Ry << ", " << Rz << ", " << Rw << ")" << std::endl;

  Quaternion ConjugateQ = RotationQ.conjugate();
  // std::cout << "Conjugate Q (" << ConjugateQ.x << ", " << ConjugateQ.y << ", " << ConjugateQ.z << ", " << ConjugateQ.w << ")" << std::endl;
  //  ConjugateQ.Normalize();
  Quaternion W = (RotationQ * (*this)) * ConjugateQ;
  // std::cout << "Partial (" << (RotationQ * (*this)).x << ", " << (RotationQ * (*this)).y << ", " << (RotationQ * (*this)).z << ", " << (RotationQ * (*this)).w << ")" << std::endl;
  // std::cout << "After multiply (" << W.x << ", " << W.y << ", " << W.z << ", " << W.w << ")" << std::endl;

  x = W.x;
  y = W.y;
  z = W.z;
}
