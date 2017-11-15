#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "GameMap.h"
#include "Camera.h"

class Game{
public:
  Game();
  ~Game();
private:
  GLFWwindow* window;
  Camera cam;
  GameMap m;
};

#endif
