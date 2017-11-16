#ifndef GAME_MESH_H
#define GAME_MESH_H

#include "Base.h"
#include "ImageLoader.h"
#include "Shader.h"

typedef struct {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
} Vertex;

typedef struct {
  GLuint texHnd;
  bool isDiffuse;
  string path;
} Texture;

class Mesh {
public:
  Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i, const ShaderProgram* p);
  ~Mesh();
  void Draw();
private:
  GLuint VAO, VBO, EBO;
  const ShaderProgram* prog;
  std::vector<Vextex> vertices;
  std::vector<Texture> textures;
  std::vector<GLuint> indices;
};

#endif
