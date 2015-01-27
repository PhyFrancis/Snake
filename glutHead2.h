#ifndef __GLUTHEAD2__
#define __GLUTHEAD2__

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "from_Yuran/OpenGL/Pipeline.h"
#include "from_Yuran/OpenGL/Camera.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

bool ReadFile(const char* pFileName, std::string& outFile)
{
  std::ifstream f(pFileName);
    
  bool ret = false;
    
  if (f.is_open()) {
    std::string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }
        
    f.close();
        
    ret = true;
  }
    
  return ret;
}

GLuint VBO = 0;
GLuint IBO = 0;
GLuint VAO = 0;
GLuint gTransLocation;
GLuint gColorLocation;
Pipeline p;
Camera cam;

const char* pVSFileName = "shader/shader.vs";
const char* pFSFileName = "shader/shader.fs";

static void RenderSceneCB()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static float scale = 0.0f;

  scale += 0.01f;
  // p.Scale(fabs(sinf(scale)));
  p.SetCamera(cam.getPos(),
              cam.getTarget(),
              cam.getUp());
  p.SetPerspectiveProj(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
  p.Scale(0.25f);
  p.WorldPos(0.0f, 0.4f * sinf(scale), 10.0f);
  p.Rotate(30.0f, scale * 90.0f, 0.0f);
  
  glUniformMatrix4fv(gTransLocation, 1, GL_TRUE, (const GLfloat*)p.getTrans());

  // Vector4f color(0.5f * (1.0f + sinf(scale)), 0.0f, 0.5f * (1.0f + cosf(scale)), 1.0);
  // glUniform4fv(gColorLocation, 1, (const GLfloat*)&color);
  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  // glEnableClientState(GL_VERTEX_ARRAY);
  glBindVertexArray(VAO);
  glCullFace(GL_BACK);

  // glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  p.WorldPos(3.0f, 0.4f * sinf(scale), 10.0f);
  glUniformMatrix4fv(gTransLocation, 1, GL_TRUE, (const GLfloat*)p.getTrans());
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  // glDisableVertexAttribArray(0);
  // glDisableClientState(GL_VERTEX_ARRAY);

  glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y) {
  cam.OnKeyboard(Key, 0.1);
}

static void KeyboardCB(unsigned char Key, int x, int y) {
  switch (Key) {
    case 'q':
      exit(0);
  }
}

static void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffer()
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  
  Vector3f Vertices[8];
  Vertices[0] = Vector3f(-1.0f, -1.0f, -1.0f);
  Vertices[1] = Vector3f(1.0f, -1.0f, -1.0f);
  Vertices[2] = Vector3f(1.0f, 1.0f, -1.0f);
  Vertices[3] = Vector3f(-1.0f, 1.0f, -1.0f);
  Vertices[4] = Vector3f(-1.0f, -1.0f, 1.0f);
  Vertices[5] = Vector3f(1.0f, -1.0f, 1.0f);
  Vertices[6] = Vector3f(1.0f, 1.0f, 1.0f);
  Vertices[7] = Vector3f(-1.0f, 1.0f, 1.0f);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  unsigned int Indices[36] = { 0, 1, 2, 2, 3, 0,
                               4, 0, 3, 3, 7, 4,
                               5, 4, 7, 7, 6, 5,
                               1, 5, 6, 6, 2, 1,
                               2, 6, 7, 7, 3, 2,
                               1, 0, 4, 4, 5, 1 };

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
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

static void CompileShaders()
{
  GLuint ShaderProgram = glCreateProgram();

  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }
    
  std::string vs, fs;

  if (!ReadFile(pVSFileName, vs)) {
    exit(1);
  }

  if (!ReadFile(pFSFileName, fs)) {
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

  // gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
  gTransLocation = glGetUniformLocation(ShaderProgram, "gTrans");
  // gColorLocation = glGetUniformLocation(ShaderProgram, "gColor");
  // assert(gScaleLocation != 0xFFFFFFFF);
  assert(gTransLocation != 0xFFFFFFFF);
  // assert(gColorLocation != 0xFFFFFFFF);
}

#endif
