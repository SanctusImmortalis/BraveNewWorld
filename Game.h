#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "GameMap.h"
#include "Shader.h"

class Game{
public:
  Game();
  ~Game();
  void mainLoop();
  unsigned int SCR_WIDTH;
  unsigned int SCR_HEIGHT;
private:
  GLFWwindow* window;
  GameMap* m;

};

#endif
