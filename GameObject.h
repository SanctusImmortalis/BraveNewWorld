#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "Base.h"

class GameObject{
protected:
  glm::vec3 position, rotation, scalefactor;
  glm::mat4 model;
  bool updateModel;
};

#endif
