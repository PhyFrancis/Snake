#include "Player.h"
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

#include <iostream>

#define BUFFSIZE 1024

void Player::setAI(std::string ai) {
  this->AI = ai;
  printf("set AI to be %s\n", ai.c_str());
}

void Player::initAI(int sizeX, int sizeY, int initX, int initY) {
  pid_t pid = (pid_t)NULL;

  int p2c[2]; pipe(p2c); // parent to child
  int c2p[2]; pipe(c2p); // child to parent

  // convetion: write to the AI process using p2c[1], and read from AI process using c2p[0]:

  pid = fork();
  if(pid < 0) {
    printf("Failed to fork AI process.\n");
    exit(1);
  }
  else if (pid == 0) {
    dup2(p2c[0], STDIN_FILENO);
    dup2(c2p[1], STDOUT_FILENO);

    std::string x = "-x " + std::to_string(sizeX);
    std::string y = "-y " + std::to_string(sizeY);
    execl(this->AI.c_str(), x.c_str(), y.c_str(), (char*) NULL);
    exit(0);
  }
  else {
    std::string init = std::to_string(initX) + " " + std::to_string(initY) + " \n";
    write(p2c[1], init.c_str(), init.size());

    std::string send;

    while(getline(std::cin, send)) {
      printf("Sent to AI: %s; ",send.c_str());
      send = send + '\n';
      write(p2c[1], send.c_str(), send.size());

      char buff[BUFFSIZE];
      read(c2p[0], buff, BUFFSIZE); // read from the process. Note that this will catch 
      std::stringstream ss; ss << buff;
      int moveX, moveY;
      ss >> moveX >> moveY;
      printf("reveive from AI: %d %d\n", moveX, moveY);
    }
  }

  // waitpid(pid, NULL, 0);
}
