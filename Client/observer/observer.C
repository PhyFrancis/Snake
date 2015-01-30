#include "Display.h"
#include "Battle.h"
#include <GL/freeglut.h>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <thread>

Display* disp;
Battle* battle;
std::mutex g_lock;
bool started;

using namespace std;

void Worker() {
  // while (battle -> genNextMove() == 0) {}
  int result = 0;
  do {
    // g_lock.lock();
    result = battle -> genNextMove();
    // g_lock.unlock();
  } while (result == 0);
}

void DisplayCB() {
  // g_lock.lock();
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
  // g_lock.unlock();
}

void IdleCB() {
  if (!started) {
    std::thread t_worker(Worker);
    t_worker.detach();
    started = true;
  }
  // battle -> genNextMove();
  // if (battle -> genNextMove() == 1) battle -> restart();
  // usleep(20 * 1000);
  // g_lock.lock();
  disp -> Render(battle -> getBoard(), battle -> getSizeX(), battle -> getSizeY());
  // g_lock.unlock();
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

void MouseCB(int button, int state, int x, int y) {
  // std::cout << "Mouse Event! " << state << std::endl;
  if (button == 3) { // Wheel up
    disp -> getCam().MoveForward(0.2);
  } else if (button == 4) { // Wheel down
    disp -> getCam().MoveBackward(0.2);
  } else if (button == 0 && state == 0) {
    // std::cout << "Mouse down" << std::endl;
    disp -> getCam().OnMouseDown(0, x, y);
  } else if (button == 0 && state == 1) {
    // std::cout << "Mouse up" << std::endl;
    disp -> getCam().OnMouseUp(0, x, y);
  }
}

void MouseMoveCB(int x, int y) {
  // std::cout << "Mouse move event!" << std::endl;
  disp -> getCam().OnMouseMove(x, y);
}

int main(int argc, char **argv) {

  started = false;
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
  glutMouseFunc(MouseCB);
  glutMotionFunc(MouseMoveCB);

  disp -> InitGLEW();
  disp -> InitVertices();
  disp -> InitShaders();

  glutMainLoop();

  return 0;
}
