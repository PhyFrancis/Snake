#include "Battle.h"
#include "Display_cyr.h"
// #include "glut/glutHead1.h"
#include <GL/freeglut.h>
#include <iostream>
#include <unistd.h>

using namespace std;

Display* disp;
Battle* battle;
// void glutInitAll(int &argc, char **argv) {
//   glutInit(&argc, argv);
//   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//   glutInitContextVersion(3, 3);
//   glutInitContextProfile(GLUT_CORE_PROFILE);
//   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//   glutInitWindowPosition(100, 100);
// }
void DisplayCB() {
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
}

void IdleCB() {
  if (battle -> genNextMove() == 1)
    battle -> restart();
  usleep(20 * 1000);
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
}

void KeyboardCB(unsigned char Key, int x, int y) {
  switch (Key) {
    case 'q':
      battle -> end();
      exit(0);
  }
}

void SpecialKeyboardCB(int Key, int x, int y) {
  disp -> getCam().OnKeyboard(Key, 0.1);
}

int main(int argc, char **argv) {
  disp = new Display(1024, 768);
  disp -> CreateWindow("Battle", &argc, argv);
  // std::cout << "Window created" << std::endl;

  battle = new Battle();
  battle ->setBoard(32, 32);
  battle ->setPlayer("AI-py/ai.py", "AI-py/ai.py");
  // battle ->setPlayer("AI-hs/AI", "AI-py/ai.py");

  glutDisplayFunc(DisplayCB);
  glutIdleFunc(IdleCB);
  glutKeyboardFunc(KeyboardCB);
  glutSpecialFunc(SpecialKeyboardCB);
  
  disp -> InitGLEW();
  disp -> InitVertices();
  disp -> InitShaders();
  
  glutMainLoop();
  return 0;
}
