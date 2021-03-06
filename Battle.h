#ifndef __BATTLE__
#define __BATTLE__

#include "Player.h"
#include <vector>

class Battle {
 public:
  typedef Pos pos_t;

  //*! constructor
  Battle(); 
  
  //*! set board size
  void setBoard(int sizeX, int sizeY) {
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_board.resize(sizeX * sizeY, 0);
  }
  
  //*! initialize two player, connect them to AI, and set their initial positions
  void setPlayer(std::string p1, std::string p2);

  //*! Add client side observer, which listen to battle output and displays the board according to Client Protocol
  void addObserver(std::string ob);

  //*! get next move from both player and update board
  int genNextMove();

  //*! get next n move from both player and update board
  int genMove(int n);

  //*! check either player has died or not
  bool checkWin(const pos_t& pos1, const pos_t& pos2);

  //*! print board
  void printBoard();

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
  std::string m_p1_name, m_p2_name;
  std::vector<int *> observer_pipes;
};

#endif
