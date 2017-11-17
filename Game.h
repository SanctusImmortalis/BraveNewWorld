#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "GameMap.h"
#include "Shader.h"

class Game{
public:
  Game();
  ~Game();
private:
  GLFWwindow* window;
  GameMap m;
  ShaderProgram entityShader;
  ShaderProgram brushShader;
};

#endif
