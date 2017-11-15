#ifndef GAME_H
#define GAME_H

#include "Base.h"
#include "GameMap.h"
#include "Camera.h"
#include "Shader.h"

class Game{
public:
  Game();
  ~Game();
private:
  GLFWwindow* window;
  Camera cam;
  GameMap m;
  std::vector<ShaderProgram> shaders;
};

#endif
