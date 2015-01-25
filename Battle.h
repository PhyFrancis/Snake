#ifndef __BATTLE__
#define __BATTLE__

#include "Player.h"
#include <vector>

class Battle {
  public:

    Battle();
    void setBoard(int sizeX, int sizeY);
    void setPlayer(std::string p1, std::string p2);
    void genMove();

  private:

    int sizeX;
    int sizeY;

    std::vector<Pos> h1;
    std::vector<Pos> h2;
    Player p1;
    Player p2;
};

#endif
