#include "Battle.h"
#include "Display.h"
#include "glut/glutHead1.h"

using namespace std;

void glutInitAll(int &argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
}

int main(int argc, char **argv) {

  glutInitAll(argc, argv);

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
  b.end();

  Display dis(&b, &argc, argv);
  dis.show();
}
