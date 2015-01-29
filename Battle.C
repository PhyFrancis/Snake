#include "Battle.h"
#include <unistd.h>
#include <iostream>

Battle::Battle() : m_sizeX(0), m_sizeY(0) {}

int
Battle::genNextMove() {
  // Make new moves according to the opponent's last move
  usleep(100000);
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

  // std::cout << "Player 1 makes move (" << newPos1.first << ", " << newPos1.second << ")" << std::endl;
  // std::cout << "Player 2 makes move (" << newPos2.first << ", " << newPos2.second << ")" << std::endl;
  
  for(int i = 0;  i < observer_pipes.size(); ++i) {
    std::string send;
    send = std::to_string(newPos1.first) + " " + std::to_string(newPos1.second) + " "
         + std::to_string(newPos2.first) + " " + std::to_string(newPos2.second) + " \n";
    write(observer_pipes[i][1], send.c_str(), send.size());
  }

  return 0;
}

//*! get next n move from both player and update board
int Battle::genMove(int n) 
{
  for(int i = 0; i < n; ++i) if(genNextMove()) return 1;
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
  m_p1_name = p1;
  m_p2_name = p2;

  // Push their initial position into the history stack
  m_history1.push_back(m_init1);
  m_history2.push_back(m_init2);

  // Set the corresponding board position
  setBoard(m_init1, 1);
  setBoard(m_init2, 2);
}

//*! Add client side observer, which listen to battle output and displays the board according to Client Protocol
void Battle::addObserver(std::string ob) 
{
  int *p2c = new int[2];
  pipe(p2c);
  observer_pipes.push_back(p2c);

  pid_t pid = fork();
  if(pid < 0) {
    printf("Failed to fork client side observer process.\n");
    exit(1);
  }
  else if (pid == 0) {
		// close un-used ends;
		close(p2c[1]);
    dup2(p2c[0], STDIN_FILENO);

    std::string x = std::to_string(m_sizeX);
    std::string y = std::to_string(m_sizeY);
    execl(ob.c_str(), ob.c_str(), x.c_str(), y.c_str(), (char*) NULL);
    exit(0);
  }
  else {
		// close un-used ends;
		close(p2c[0]);
    // dup2(p2c[1], STDOUT_FILENO);
  }
}

bool
Battle::checkWin(const pos_t& pos1, const pos_t& pos2)
{
  if (pos1 == Pos(-1, -1)) {
    std::cout << "END" << std::endl;
    std::cout << "Player 2 wins!" << std::endl;
    return true;
  }
  if (pos2 == Pos(-1, -1)) {
    std::cout << "END" << std::endl;
    std::cout << "Player 1 wins!" << std::endl;
    return true;
  }
  return false;
}

void
Battle::end() 
{
  m_p1.end();
  m_p2.end();
  m_history1.clear();
  m_history2.clear();
  m_board.clear();
  m_board.resize(m_sizeX * m_sizeY, 0.0);
  for(int i = 0; i < observer_pipes.size(); ++i) delete [] observer_pipes[i];
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

Battle::~Battle() 
{
  end();
}

void
Battle::restart() {
  end();
  setPlayer(m_p1_name, m_p2_name);
}
