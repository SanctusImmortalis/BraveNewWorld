#ifndef GAME_MESH_H
#define GAME_MESH_H

#include "Base.h"
#include "Shader.h"
#include "DrawData.h"

class Mesh {
public:
  Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i);
  ~Mesh();
  void draw(ShaderProgram* prog);
  std::vector<glm::vec3> getVertices();

  ShaderProgram prog;
private:
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<GLuint> indices;
};

#endif
