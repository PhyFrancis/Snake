#include "Battle.h"

#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
  Player p;
  p.setAI("AI/ai.py");
  p.initAI(50,50,4,4);

  pair<int, int> move(10,10);
  for(int i = 10; i < 20; i++) {
    move.first = i;
    p.genMove(move);
  }

  p.end();
  cout<<"start second run: "<<endl;
  p.initAI(50,50,4,4);
  for(int i = 10; i < 20; i++) {
    move.first = i;
    p.genMove(move);
  }
 
  // pid_t pid = (pid_t)NULL;

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
