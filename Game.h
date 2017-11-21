#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "ImageLoader.h"
#include "GameMap.h"
#include "Shader.h"

class Game{
public:
  Game();
  ~Game();
  void mainLoop();
private:
  GLFWwindow* window;
  GameMap* m;

};

#endif
