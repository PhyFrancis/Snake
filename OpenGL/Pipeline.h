#ifndef  _PIPELINE_H_
#define  _PIPELINE_H_

#include "dataTypes.h"

class Pipeline
{
 public:
  Pipeline()
      : m_scale(1.0f, 1.0f, 1.0f),
        m_worldPos(0.0f, 0.0f, 0.0f),
        m_rotation(0.0f, 0.0f, 0.0f) {}
               
  ~Pipeline() {}

  void Scale(float s) { Scale(s, s, s); }
  void Scale(const Vector3f& scale) { Scale(scale.x, scale.y, scale.z); }
  void Scale(float scaleX, float scaleY, float scaleZ) {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
    m_scale.z = scaleZ;
  }

  void WorldPos(const Vector3f& pos) { m_worldPos = pos; }
  void WorldPos(float x, float y, float z) {
    m_worldPos.x = x;
    m_worldPos.y = y;
    m_worldPos.z = z;
  }

  void Rotate(const Vector3f& r) { Rotate(r.x, r.y, r.z); }
  void Rotate(float rx, float ry, float rz) {
    m_rotation.x = rx;
    m_rotation.y = ry;
    m_rotation.z = rz;
  }

  const Matrix4f* getTrans() {
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans, PerspTrans, CamRotationTrans, CamPosTrans;
    
    ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    RotateTrans.InitRotateTransform(m_rotation.x, m_rotation.y, m_rotation.z);
    TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
    PerspTrans.InitPerspectiveProj(m_persProj);
    CamPosTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    CamRotationTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    // std::cout << PerspTrans.m[1][1] << std::endl;
    m_transformation = PerspTrans * CamRotationTrans * CamPosTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &m_transformation;
  }

  void SetPerspectiveProj(float fov, float width, float height, float znear, float zfar) {
    m_persProj.FOV = fov;
    m_persProj.Width = width;
    m_persProj.Height = height;
    m_persProj.zNear = znear;
    m_persProj.zFar = zfar;
  }

  void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up) {
    m_camera.Pos = pos;
    m_camera.Target = target;
    m_camera.Up = up;
  }

 private:
  Vector3f m_scale;
  Vector3f m_worldPos;
  Vector3f m_rotation;
  Matrix4f m_transformation;
  PersProjInfo m_persProj;

  struct {
    Vector3f Pos;
    Vector3f Target;
    Vector3f Up;
  } m_camera;
  // Matrix4f m_perspProj;
}; // ----- end of class Pipeline -----


#endif   // _PIPELINE_H_
