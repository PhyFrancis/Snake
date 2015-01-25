#ifndef __PLAYER__
#define __PLAYER__

#include <utility>
#include <string>

typedef std::pair<int, int> Pos;

class Player {

  public:
    
    //*! set AI to be the filename of the excutable
    void setAI(std::string ai);

    //*! set game board size and initial position
    void initAI(int sizeX, int sizeY, int initX, int initY);

    //*! generate move according to rival's move
    Pos genMove(const Pos &rivalMove);

  private:
    std::string AI;
};

#endif
