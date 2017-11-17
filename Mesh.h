#ifndef GAME_MESH_H
#define GAME_MESH_H

#include "Base.h"
#include "ImageLoader.h"
#include "Shader.h"
#include "DrawData.h"

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
