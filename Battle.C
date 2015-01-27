#include "Battle.h"
#include <iostream>

Battle::Battle() : m_sizeX(0), m_sizeY(0) {}

int
Battle::genNextMove() {
  // Make new moves according to the opponent's last move
  pos_t newPos1 = m_p1.genMove(m_history2.back());
  pos_t newPos2 = m_p2.genMove(m_history1.back());
  if (checkWin(newPos1, newPos2)) {
    std::cout << "Game ends!" << std::endl;
    return 1;
  }

  m_history1.push_back(newPos1);
  m_history2.push_back(newPos2);

  // Set the new board positions
  setBoard(newPos1, 1);
  setBoard(newPos2, 2);

#define debug
#ifdef debug
  std::cout << "Player 1 makes move (" << newPos1.first << ", " << newPos1.second << ")" << std::endl;
  std::cout << "Player 2 makes move (" << newPos2.first << ", " << newPos2.second << ")" << std::endl;
  // printBoard();
#endif

  return 0;
}

void
Battle::setPlayer(std::string p1, std::string p2) {
  // Set initial positions for the players
  m_init1 = pos_t(m_sizeX / 3, m_sizeY / 3);
  m_init2 = pos_t(m_sizeX * 2 / 3, m_sizeY * 2 / 3);

  // Set AI and initialize them
  m_p1.setAI(p1);
  m_p2.setAI(p2);
  m_p1.initAI(m_sizeX, m_sizeY, m_init1.first, m_init1.second);
  m_p2.initAI(m_sizeX, m_sizeY, m_init2.first, m_init2.second);

  // Push their initial position into the history stack
  m_history1.push_back(m_init1);
  m_history2.push_back(m_init2);

  // Set the corresponding board position
  setBoard(m_init1, 1);
  setBoard(m_init2, 2);
}

bool
Battle::checkWin(const pos_t& pos1, const pos_t& pos2)
{
  if (pos1 == Pos(-1, -1)) {
    std::cout << "Player 2 wins!" << std::endl;
    return true;
  }
  if (pos2 == Pos(-1, -1)) {
    std::cout << "Player 1 wins!" << std::endl;
    return true;
  }
  // if (m_board[toIndex(m_history1.back())] != 0 || m_board[toIndex(m_history2.back())] != 0)
  //   return true;
  // else
  return false;
}

void
Battle::end() 
{
  m_p1.end();
  m_p2.end();
}

void
Battle::printBoard()
{
  for (int j = 0; j < m_sizeY; j++) {
    for (int i = 0; i < m_sizeX; i++) {
      int index = i + j * m_sizeX;
      std::cout << static_cast<int>(m_board[index]) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void
Battle::printBoardFancy()
{
  for (int j = 0; j < m_sizeY; j++) {
    for (int i = 0; i < m_sizeX; i++) {
      int index = i + j * m_sizeX;
      std::cout << static_cast<int>(m_board[index]) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

Battle::~Battle() 
{
  end();
}
