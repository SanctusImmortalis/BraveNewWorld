#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "Base.h"
#include "Shader.h"
#include "Entity.h"
#include "Camera.h"
#include "GameObject.h"
#include "DrawData.h"

class Brush : public GameObject{
public:
  Brush(std::vector<Vertex> v, Texture diff, Texture spec, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
  ~Brush();
  void draw();
  static ShaderProgram brushShader;
private:
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  Texture diffuse, specular;
  std::vector<GLuint> indices;
};

class GameMap{
private:
  Camera cam;
  std::vector<Entity> ents;
  std::vector<Brush> brushes;
};

#endif
