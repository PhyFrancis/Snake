#ifndef __DISPLAY__
#define __DISPLAY__

#include "Battle.h"

class Display {
  public:
    //*! constructor
    Display(Battle *_bat, int *argc, char **argv);

    //*! display history array in battle
    void show();

  private:
    int *argc;
    char **argv;

    Battle *bat;
};

#endif
