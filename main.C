#include "Battle.h"

#include <unistd.h>
#include <iostream>
#include <sstream>

using namespace std;


int main() {
  // Player p;
  // p.setAI("AI/ai.py");
  // p.initAI(50,50,4,4);

  // pair<int, int> move(10,10);
  // for(int i = 10; i < 20; i++) {
  //   move.first = i;
  //   pair<int, int> m = p.genMove(move);
  //   cout<<"my move: "<<move.first<<" "<<move.second<<"; AI's move: "<<m.first<<" "<<m.second<<endl;
  // }

  // p.end();
  // cout<<"start second run: "<<endl;
  // p.initAI(50,50,4,4);
  // for(int i = 10; i < 20; i++) {
  //   move.second = i;
  //   pair<int, int> m = p.genMove(move);
  //   cout<<"my move: "<<move.first<<" "<<move.second<<"; AI's move: "<<m.first<<" "<<m.second<<endl;
  // }
  

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
}
