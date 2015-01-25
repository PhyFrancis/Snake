#ifndef __PLAYER__
#define __PLAYER__

#include <utility>
#include <string>

typedef std::pair<int, int> Pos;

class Player {

  public:

		//*! constructor
		Player();
    
    //*! set AI to be the filename of the excutable
    void setAI(std::string ai);

    //*! set game board size and initial position
    void initAI(int sizeX, int sizeY, int initX, int initY);

    //*! generate move according to rival's move
    Pos genMove(const Pos &rivalMove);

		//*! destructor
		~Player();

  private:

		//*! AI excutable name
    std::string AI;

		//*! file descriptor pipe to communicate with AI bot
		int p2c[2]; 
		int c2p[2]; 

		//*! is initialized
		bool initialized;

		//*! to store the pid of AI process
		pid_t pid;
};

#endif
