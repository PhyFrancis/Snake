#ifndef __BATTLE__
#define __BATTLE__

#include <vector>
#include <utility>
#include <string>

class Battle {
 public:
  typedef std::pair<int,int> pos_t;

  //*! constructor
  Battle(); 
  
  //*! set board size
  void setBoard(int sizeX, int sizeY) {
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_board.resize(sizeX * sizeY, 0);
  }
  
  //*! get next move from both player and update board
  int genNextMove();

  //*! disconnect from AI
  void end();

  void restart();

  //*! destructor
  ~Battle();

  const char* getBoard() { return m_board.data(); }
  int getSizeX() { return m_sizeX; }
  int getSizeY() { return m_sizeY; }

 private:

  int toIndex (const pos_t& pos) {
    return pos.first + m_sizeX * pos.second;
  }

  void setBoard (const pos_t& pos, int p) {
    m_board[toIndex(pos)] = p;
  }

  int m_sizeX, m_sizeY;

  std::vector<char> m_board;
};

#endif
