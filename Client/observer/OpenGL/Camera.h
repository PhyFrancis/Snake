#ifndef  _CAMERA_H_
#define  _CAMERA_H_

#include "dataTypes.h"
#include <iostream>

class Camera {
 public:
  Camera() {}
  Camera(int windowW, int windowH, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
      : m_win_width(windowW), m_win_height(windowH), m_pos(pos), m_target(target), m_up(up) {
    m_angleH = 0.0f;
    m_angleV = 0.0f;
    m_mouse_prev_x = 0;
    m_mouse_prev_y = 0;
    m_moving = false;
    // m_target.normalize();
    m_up.normalize();
  }
      
  ~Camera() {}

  bool OnKeyboard (int key, float StepSize) {
    bool Ret = false;

    switch (key) {

      case GLUT_KEY_UP:
        {
          MoveForward(StepSize * 4);
          Ret = true;
        }
        break;

      case GLUT_KEY_DOWN:
        {
          MoveBackward(StepSize * 4);
          Ret = true;
        }
        break;

      case GLUT_KEY_LEFT:
        {
          MoveLeft(StepSize);
          Ret = true;
        }
        break;

      case GLUT_KEY_RIGHT:
        {
          MoveRight(StepSize);
          Ret = true;
        }
        break;
    }

    return Ret;
  }

  void OnMouseDown (int button, int x, int y) {
    if (button == 0) { // Left button down, start moving
      if (!m_moving) m_moving = true;
      m_mouse_prev_x = x;
      m_mouse_prev_y = y;
    }
  }

  void OnMouseUp (int button, int x, int y) {
    if (button == 0) { // Left button up, stop moving
      if (m_moving) m_moving = false;
      m_mouse_rel_x = x;
      m_mouse_rel_y = y;
    }
  }

  void OnMouseMove (int x, int y) {
    if (!m_moving) OnMouseDown(1, x, y);
    int dx = x - m_mouse_prev_x;
    int dy = y - m_mouse_prev_y;
    m_mouse_prev_x = x;
    m_mouse_prev_y = y;

    m_angleH += (float)dx / 20.0f;
    m_angleV += (float)dy / 20.0f;
    // std::cout << "Angle H is " << m_angleH << ", angle V is " << m_angleV << std::endl;

    Update();
  }

  void MoveForward (float step) {
    m_pos += (m_target - m_pos).normalize() * step;
  }

  void MoveBackward (float step) {
    m_pos -= (m_target - m_pos).normalize() * step;
  }

  void MoveLeft (float step) {
    Vector3f Left = (m_target - m_pos).normalize().cross(m_up);
    Left.normalize();
    Left *= step;
    m_pos += Left;
    m_target += Left;
  }

  void MoveRight (float step) {
    Vector3f Right = m_up.cross((m_target - m_pos).normalize());
    Right.normalize();
    Right *= step;
    m_pos += Right;
    m_target += Right;
  }
  
  void OnRender () {
  }

  const Vector3f& getPos() const { return m_pos; }
  const Vector3f& getTarget() const { return m_target; }
  const Vector3f& getUp() const { return m_up; }

 private:
  void Init() {
  }

  void Update() {
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(0.0f, 0.0f, 1.0f);
    // Vector3f View = (m_target - m_pos).normalize();
    View.rotate(m_angleH, Vaxis);
    // std::cout << "View is (" << View.x << ", " << View.y << ", " << View.z << ")" << std::endl;
    View.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.cross(View);
    Haxis.normalize();
    View.rotate(m_angleV, Haxis);
    // std::cout << "View is (" << View.x << ", " << View.y << ", " << View.z << ")" << std::endl;

    // Vector3f direction = (m_target - m_pos).normalize();
    Vector3f direction = View;
    direction.normalize();
    float distance = (m_target - m_pos).length();
    // std::cout << "Length is " << distance << std::endl;
    m_pos = (m_target - direction * distance);
    // std::cout << "Direction is (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
    // std::cout << "Target is (" << m_target.x << ", " << m_target.y << ", " << m_target.z << ")" << std::endl;
    // std::cout << "Pos is (" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")" << std::endl;

    m_up = direction.cross(Haxis);
    m_up.normalize();
  }
  
  int m_win_width, m_win_height;
  float m_angleH, m_angleV;
  int m_mouse_prev_x, m_mouse_prev_y;
  int m_mouse_rel_x, m_mouse_rel_y;
  
  Vector3f m_pos;
  Vector3f m_target;
  Vector3f m_up;
  bool m_moving;
}; // ----- end of class Camera -----


#endif   // _CAMERA_H_
