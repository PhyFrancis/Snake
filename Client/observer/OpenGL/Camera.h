#ifndef  _CAMERA_H_
#define  _CAMERA_H_

#include "dataTypes.h"

class Camera {
 public:
  Camera() {}
  Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up) :
      m_pos(pos), m_target(target), m_up(up) {}
      
  ~Camera() {}

  bool OnKeyboard (int key, float StepSize) {
    bool Ret = false;

    switch (key) {

    case GLUT_KEY_UP:
    {
        m_pos += (m_target * StepSize * 4);
        Ret = true;
    }
    break;

    case GLUT_KEY_DOWN:
    {
        m_pos -= (m_target * StepSize * 4);
        Ret = true;
    }
    break;

    case GLUT_KEY_LEFT:
    {
        Vector3f Left = m_target.cross(m_up);
        Left.normalize();
        Left *= StepSize;
        m_pos += Left;
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        Vector3f Right = m_up.cross(m_target);
        Right.normalize();
        Right *= StepSize;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
  }

  const Vector3f& getPos() const { return m_pos; }
  const Vector3f& getTarget() const { return m_target; }
  const Vector3f& getUp() const { return m_up; }

 private:
  Vector3f m_pos;
  Vector3f m_target;
  Vector3f m_up;
}; // ----- end of class Camera -----


#endif   // _CAMERA_H_
