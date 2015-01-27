#include "Display_cyr.h"
#include <iostream>
#include <cstring>

Display::Display(int w_width, int w_height) :
    m_width(w_width), m_height(w_height),
    m_cam(Vector3f(-3.0f, 0.0f, -6.0f),
          Vector3f(0.3f, 0.0f, 1.0f),
          Vector3f(0.0f, 1.0f, 0.0f)) {
  // Set the camera and perspective of the pipeline
  m_pipe.SetCamera(m_cam.getPos(),
                   m_cam.getTarget(),
                   m_cam.getUp());
  m_pipe.SetPerspectiveProj(30.0f, m_width, m_height, 1.0f, 1000.0f);

}

Display::~Display() {}

void
Display::CreateWindow(const std::string& title, int* argc, char** argv) {
  std::cout << "Start create window!" << std::endl;
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  // glutInitContextFlags(GLUT_DEBUG);
  glutInitWindowSize(m_width, m_height);
  glutInitWindowPosition(100, 100);

  glutCreateWindow(title.c_str());
}

void
Display::InitGLEW() {
  glewExperimental = GL_TRUE;
  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return;
  }
  glEnable(GL_CULL_FACE);

  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
}

void
Display::InitVertices() {
  // Vertex array object
  glGenVertexArrays(1, &m_cube_vao);
  glBindVertexArray(m_cube_vao);
  glEnableVertexAttribArray(0);

  // Vertices for a cube
  Vector3f Vertices[m_vert_n];
  Vertices[0] = Vector3f(-1.0f, -1.0f, -1.0f);
  Vertices[1] = Vector3f(1.0f, -1.0f, -1.0f);
  Vertices[2] = Vector3f(1.0f, 1.0f, -1.0f);
  Vertices[3] = Vector3f(-1.0f, 1.0f, -1.0f);
  Vertices[4] = Vector3f(-1.0f, -1.0f, 1.0f);
  Vertices[5] = Vector3f(1.0f, -1.0f, 1.0f);
  Vertices[6] = Vector3f(1.0f, 1.0f, 1.0f);
  Vertices[7] = Vector3f(-1.0f, 1.0f, 1.0f);

  // Vertex Buffer object
  glGenBuffers(1, &m_cube_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Indices for drawing a cube
  unsigned int Indices[m_id_n] = { 0, 1, 2, 2, 3, 0,
                                   4, 0, 3, 3, 7, 4,
                                   5, 4, 7, 7, 6, 5,
                                   1, 5, 6, 6, 2, 1,
                                   2, 6, 7, 7, 3, 2,
                                   1, 0, 4, 4, 5, 1 };

  // Index Buffer object
  glGenBuffers(1, &m_cube_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  std::cout << "Vertices initialized" << std::endl;
}

void
Display::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = pShaderText;
  GLint Lengths[1];
  Lengths[0]= std::strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }

  glAttachShader(ShaderProgram, ShaderObj);
}

void
Display::InitShaders() {
  GLuint ShaderProgram = glCreateProgram();

  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }
    
  std::string vs, fs;

  if (!ReadFile(m_vs_file.c_str(), vs)) {
    exit(1);
  }

  if (!ReadFile(m_fs_file.c_str(), fs)) {
    exit(1);
  }

  AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
  AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glUseProgram(ShaderProgram);

  // Get the locations of uniform variables
  m_trans = glGetUniformLocation(ShaderProgram, "gTrans");
  m_color = glGetUniformLocation(ShaderProgram, "gColor");
  assert(m_trans != 0xFFFFFFFF);
  assert(m_color != 0xFFFFFFFF);

  std::cout << "Shaders initialized!" << std::endl;
}

void
Display::Render(const char* board, int sizeX, int sizeY) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int j = 0; j < sizeY + 2; j++) {
    for (int i = 0; i < sizeX + 2; i++) {
      if (i == 0 || i == sizeX + 1 || j == 0 || j == sizeY + 1) {
        // Border
        m_pipe.Scale(0.05f);
        // m_pipe.Rotate(0.0f, 30.0f, 0.0f);
        m_pipe.WorldPos((i - sizeX / 2) * 0.12f, (j - sizeY / 2) * 0.12f, 10.0f);
        // m_pipe.WorldPos(i * 0.12f, j * 0.12f, 20.0f);
        m_pipe.SetCamera(m_cam.getPos(),
                         m_cam.getTarget(),
                         m_cam.getUp());
        m_pipe.SetPerspectiveProj(30.0f, m_width, m_height, 1.0f, 1000.0f);
        glUniformMatrix4fv(m_trans, 1, GL_TRUE, (const GLfloat*)m_pipe.getTrans());
        Vector4f color = Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
        glUniform4fv(m_color, 1, (const GLfloat*)&color);
      
        glBindVertexArray(m_cube_vao);
        // glCullFace(GL_BACK);

        glDrawElements(GL_TRIANGLES, m_id_n, GL_UNSIGNED_INT, 0);
      } else {
        int idx = i - 1 + (j - 1) * sizeX;
        // This position is not occupied
        if (board[idx] == 0) continue;
        // std::cout << "(" << i << ", " << j << ")" << std::endl;
        // std::cout << "(" << i << ", " << j << ")" << std::endl;

        m_pipe.Scale(0.05f);
        // m_pipe.Rotate(0.0f, 30.0f, 0.0f);
        m_pipe.WorldPos((i - sizeX / 2) * 0.12f, (j - sizeY / 2) * 0.12f, 10.0f);
        // m_pipe.WorldPos(i * 0.12f, j * 0.12f, 20.0f);
        m_pipe.SetCamera(m_cam.getPos(),
                         m_cam.getTarget(),
                         m_cam.getUp());
        m_pipe.SetPerspectiveProj(30.0f, m_width, m_height, 1.0f, 1000.0f);
        glUniformMatrix4fv(m_trans, 1, GL_TRUE, (const GLfloat*)m_pipe.getTrans());
        Vector4f color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        if (board[idx] == 1) // Red for player 1
          color = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
        else if (board[idx] == 2) // Blue for player 2
          color = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

        glUniform4fv(m_color, 1, (const GLfloat*)&color);
      
        glBindVertexArray(m_cube_vao);
        // glCullFace(GL_BACK);

        glDrawElements(GL_TRIANGLES, m_id_n, GL_UNSIGNED_INT, 0);
      }
    }
  }

  // m_pipe.Scale(0.25f);
  // // m_pipe.WorldPos((i - sizeX / 2) * 0.12f, (j - sizeY / 2) * 0.12f, 20.0f);
  // m_pipe.WorldPos(0.0f, 0.0f, 10.0f);
  // m_pipe.SetCamera(m_cam.getPos(),
  //                  m_cam.getTarget(),
  //                  m_cam.getUp());
  // m_pipe.SetPerspectiveProj(30.0f, m_width, m_height, 1.0f, 1000.0f);
  // m_pipe.Rotate(30.0f, 60.0f, 0.0f);
  // glUniformMatrix4fv(m_trans, 1, GL_TRUE, (const GLfloat*)m_pipe.getTrans());
  // glBindVertexArray(m_cube_vao);
  //     // glCullFace(GL_BACK);

  // glDrawElements(GL_TRIANGLES, m_id_n, GL_UNSIGNED_INT, 0);
  // std::cout << "Render!" << std::endl;
  glutSwapBuffers();
}
