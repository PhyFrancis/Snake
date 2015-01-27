#include "Battle.h"
#include "Display.h"

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "from_Yuran/OpenGL/Pipeline.h"
#include "from_Yuran/OpenGL/Camera.h"

using namespace std;

int main(int argc, char **argv) {
  string AI1 = "AI-py/ai.py";
  string AI2 = "AI-hs/AI";
  const int board_size = 20;

  Battle b;
  b.setBoard(board_size, board_size);
  b.setPlayer(AI1, AI2);
  for (int i = 0; i < 100; i++) {
    if (b.genNextMove() == 1) break;
  }
  b.printBoard();

  Display dis(&b, &argc, argv);
  // dis.show();
}
