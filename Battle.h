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
    m_board.resize(sizeX * sizeY, 0);
  }
  
  void setPlayer(std::string p1, std::string p2);
  
  int genNextMove();

  bool checkWin(const pos_t& pos1, const pos_t& pos2);

  void printBoard();

 private:

  int toIndex (const pos_t& pos) {
    return pos.first + m_sizeX * pos.second;
  }

  bool isOccupied (const pos_t& pos) {
    return (m_board[toIndex(pos)] != 0);
  }

  void setBoard (const pos_t& pos, int p) {
    m_board[toIndex(pos)] = p;
  }

  int m_sizeX, m_sizeY;

  pos_t m_init1, m_init2;

  std::vector<pos_t> m_history1;
  std::vector<pos_t> m_history2;
  std::vector<char> m_board;
  
  Player m_p1, m_p2;
};

#endif
