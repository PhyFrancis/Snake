#ifndef __BATTLE__
#define __BATTLE__

#include "Player.h"
#include <vector>

class Battle {
 public:
  typedef Pos pos_t;

  Battle(); 
  
  void setBoard(int sizeX, int sizeY) {
    m_sizeX = sizeX;
    m_sizeY = sizeY;
  }
  
  void setPlayer(std::string p1, std::string p2) {
    m_p1.setAI(p1);
    m_p2.setAI(p2);
    m_p1.initAI(m_sizeX, m_sizeY, m_sizeX / 3, m_sizeY / 3);
    m_p2.initAI(m_sizeX, m_sizeY, m_sizeX * 2 / 3, m_sizeY * 2 / 3);

    m_history1.push_back(Pos)
  }
  
  void genNextMove();

 private:

  int m_sizeX;
  int m_sizeY;

  std::vector<pos_t> m_history1;
  std::vector<pos_t> m_history2;
  Player m_p1;
  Player m_p2;
};

#endif
