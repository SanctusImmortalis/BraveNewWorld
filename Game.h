#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "Entity.h"

class Game{
public:
  Game();
  ~Game();
private:
  GLFWwindow* window;
  Entity ents[100];
};

#endif
