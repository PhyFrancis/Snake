#include "Battle.h"

#include <unistd.h>
#include <iostream>
#include <sstream>

using namespace std;

template <int DIM>
struct POS_DIM {};

template <>
struct POS_DIM<2> {
  typedef Pos type;
};


int main() {
<<<<<<< Updated upstream
  Player p;
  p.setAI("AI/ai.py");
  p.initAI(50,50,4,4);

  pair<int, int> move(10,10);
  for(int i = 10; i < 20; i++) {
    move.first = i;
    pair<int, int> m = p.genMove(move);
    cout<<"my move: "<<move.first<<" "<<move.second<<"; AI's move: "<<m.first<<" "<<m.second<<endl;
  }

  p.end();
  cout<<"start second run: "<<endl;
  p.initAI(50,50,4,4);
  for(int i = 10; i < 20; i++) {
    move.second = i;
    pair<int, int> m = p.genMove(move);
    cout<<"my move: "<<move.first<<" "<<move.second<<"; AI's move: "<<m.first<<" "<<m.second<<endl;
  }
 
=======
  // Player p;
  // p.setAI("AI/ai.py");
  // p.initAI(50,50,4,4);

  // std::string send;
  // while (getline(std::cin, send)) {
  //   std::stringstream ss;
  //   ss << send;
  //   int x, y;
  //   ss >> x >> y;
  //   POS_DIM<2>::type new_move = p.genMove(Pos(x, y));
  // }
>>>>>>> Stashed changes
  // pid_t pid = (pid_t)NULL;
  Battle b;
  for (int i = 0; )

  // int c2p[2]; pipe(c2p); // child to parent

  // pid = fork();

  // if(pid == 0) {
  //   write(c2p[1], "Hello World!", 12);
  // }
  // else {
  //   char buf[4096*2];
  //   read(c2p[0], buf, sizeof(buf)); // read from the process. Note that this will catch 
  //   printf("%s\n",buf);
  // }
}
