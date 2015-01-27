#ifndef  _DISPLAY_H_
#define  _DISPLAY_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cassert>
#include <cmath>
#include "OpenGL/dataTypes.h"
#include "OpenGL/Pipeline.h"
#include "OpenGL/Camera.h"
#include "utility.h"

class Display {
 public:
  Display(int width, int height);
  ~Display();

  void CreateWindow(const std::string& title, int* argc, char** argv);
  void InitGLEW();
  void InitVertices();
  void InitShaders();

  void Render(const char* board, int sizeX, int sizeY);

  Camera& getCam () { return m_cam; }

 private:
  void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
  
  int m_width, m_height;
  const int m_vert_n = 8;
  const int m_id_n = 36;

  GLuint m_cube_vao, m_cube_vbo, m_cube_ibo;
  GLuint m_trans;
  GLuint m_color;

  Pipeline m_pipe;
  Camera m_cam;

  std::string m_vs_file = "shader.vs";
  std::string m_fs_file = "shader.fs";
}; // ----- end of class Display -----


#endif   // _DISPLAY_H_
