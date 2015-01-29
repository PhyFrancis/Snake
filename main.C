#include "Battle.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  Battle* battle = new Battle();
  battle ->setBoard(32, 32);
  battle ->setPlayer("AI-py/ai.py", "AI-py/ai.py");
  battle ->addObserver("Client/observer/observer.x");

  battle ->genMove(1000);
  battle ->end();
}
