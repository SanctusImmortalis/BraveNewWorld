#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "Mesh.h"

class Model{
public:
  Model(char* path, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
private:
  glm::mat4 model;
};

#endif
