#ifndef GAME_MESH_H
#define GAME_MESH_H

#include "Base.h"
#include "ImageLoader.h"
#include "Shader.h"
#include "DrawData.h"

class Mesh {
public:
  Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i);
  Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i, ShaderProgram p);
  ~Mesh();
  void draw(glm::mat4 model);
  void enableCustomShader();
  void disableCustomShader();
  void toggleCustomShader();
  static ShaderProgram entityShader;
  ShaderProgram prog;
private:
  GLuint VAO, VBO, EBO;
  bool customShader;
  std::vector<Vextex> vertices;
  std::vector<Texture> textures;
  std::vector<GLuint> indices;
};

#endif
