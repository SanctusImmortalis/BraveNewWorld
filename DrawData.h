#ifndef DRAW_DATA_H
#define DRAW_DATA_H

#include "Base.h"

typedef struct {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
} Vertex;

typedef struct {
  GLuint texHnd;
  bool isDiffuse;
  std::string path;
} Texture;

#endif
