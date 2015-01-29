#include "Display.h"
#include "Battle.h"
#include <GL/freeglut.h>
#include <iostream>
#include <unistd.h>

Display* disp;
Battle* battle;

using namespace std;

void DisplayCB() {
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
}

void IdleCB() {
  if (battle -> genNextMove() == 1) battle -> restart();
  usleep(20 * 1000);
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
}

void KeyboardCB(unsigned char Key, int x, int y) {
  switch (Key) {
    case 'q':
      exit(0);
  }
}

void SpecialKeyboardCB(int Key, int x, int y) {
  disp -> getCam().OnKeyboard(Key, 0.1);
}

int main(int argc, char **argv) {

  if(argc < 3) cout<<"Please call the function as: bin x_size y_size!"<<endl;
  int xSize = atoi(argv[1]);
  int ySize = atoi(argv[2]);

  disp = new Display(1024, 768);
  disp -> CreateWindow("Battle", &argc, argv);

  battle = new Battle();
  battle ->setBoard(xSize, ySize);

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
