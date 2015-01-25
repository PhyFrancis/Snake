#include "Player.h"
#include <sstream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

// #include <iostream>

#define BUFFSIZE 1024

Pos Player::die = std::make_pair(-1,-1);

Player::Player() {
	initialized = false;
}

void Player::setAI(std::string ai) {
  this->AI = ai;
  printf("set AI to be %s\n", ai.c_str());
}

void Player::initAI(int sizeX, int sizeY, int initX, int initY) {
  if(initialized) end();
  pipe(p2c); // parent to child
  pipe(c2p); // child to parent

  // convention: write to AI process using p2c[1], and read from AI process using c2p[0]:
	
  pid = fork();
  if(pid < 0) {
    printf("Failed to fork AI process.\n");
    exit(1);
  }
  else if (pid == 0) {
		// close un-used ends;
		close(p2c[1]);
		close(c2p[0]);

    dup2(p2c[0], STDIN_FILENO);
    dup2(c2p[1], STDOUT_FILENO);

    std::string x = "-x " + std::to_string(sizeX);
    std::string y = "-y " + std::to_string(sizeY);
    execl(this->AI.c_str(), this->AI.c_str(), x.c_str(), y.c_str(), (char*) NULL);
    exit(0);
  }
  else {
		// close un-used ends;
		close(p2c[0]);
		close(c2p[1]);

    std::string init = std::to_string(initX) + " " + std::to_string(initY) + " \n";
    write(p2c[1], init.c_str(), init.size());
  }

	initialized = true;
}

Pos Player::genMove(const Pos &rivalMove) {
  
  int rival_X = rivalMove.first;
  int rival_Y = rivalMove.second;

  std::string send;
  send = std::to_string(rival_X) + " " + std::to_string(rival_Y) + " \n";

  write(p2c[1], send.c_str(), send.size()); // write to AI process

  char buff[BUFFSIZE];
  read(c2p[0], buff, BUFFSIZE); // read from AI process. 
  std::stringstream ss; ss << buff;
  if (ss.str().substr(0,3) == "die") return die;
  int moveX, moveY;
  ss >> moveX >> moveY;
  return std::make_pair(moveX, moveY);
}

void Player::end() {
  if(!initialized) return;
  kill(pid, 9);
  initialized = false;
}

Player::~Player() {
  end();
	close(p2c[0]); close(p2c[1]); 
	close(c2p[0]); close(c2p[1]); 
}
