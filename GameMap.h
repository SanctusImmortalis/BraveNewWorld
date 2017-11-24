#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "Base.h"
#include "Shader.h"
#include "Brush.h"
#include "Entity.h"
#include "Camera.h"
#include "GameObject.h"
#include "DrawData.h"

//ShaderProgram* bs();



class GameMap{
public:
  GameMap(void (*func)(GameMap* m)) {func(this);}
  GLuint matrices, lights;
  Camera cam;
  ShaderProgram* shaders[3];
  int shadernum;
  Entity* ents[7];
  int entnum;
  Brush* brushes[37];
  int brushnum;
};

#endif
