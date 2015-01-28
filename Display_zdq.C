#include "Display_zdq.h"
#include "glut/glutHead2.h"

Display::Display(Battle *_bat, int *argc, char **argv): bat(_bat) {
  this->argc = argc;
  this->argv = argv;
}

void Display::show() {
  glutCreateWindow("Tutorial 03");
  InitializeGlutCallbacks();
  glewExperimental = GL_TRUE;

  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return;
  }
  glEnable(GL_CULL_FACE);

  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

  // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Set initial camera
  cam = Camera(Vector3f(0.0f, 0.0f, -3.0f),
               Vector3f(0.0f, 0.0f, 1.0f),
               Vector3f(0.0f, 1.0f, 0.0f));

  CreateVertexBuffer();
  CompileShaders();
  glutMainLoop();
}
