#include "Battle.h"
#include <iostream>
#include <sstream>

Battle::Battle() : m_sizeX(0), m_sizeY(0) {}

int
Battle::genNextMove() {
  std::string move;
  std::getline(std::cin, move);
  if(move.substr(0,3) == "END") {
    std::string gameEndInfo;
    std::getline(std::cin, gameEndInfo);
    std::cout<<gameEndInfo<<std::endl;
    return 1;
  }
  std::stringstream ss(move);
  
  int p1_x, p1_y, p2_x, p2_y;
  ss>> p1_x >> p1_y >> p2_x >> p2_y;
  setBoard(pos_t(p1_x, p1_y), 1);
  setBoard(pos_t(p2_x, p2_y), 2);
  return 0;
}

void
Battle::end() 
{
  m_board.clear();
  m_board.resize(m_sizeX * m_sizeY, 0.0);
}

Battle::~Battle() 
{
  end();
}

void
Battle::restart() {
  end();
}
