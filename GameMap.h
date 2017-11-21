#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "Base.h"
#include "Shader.h"
#include "Entity.h"
#include "Camera.h"
#include "GameObject.h"
#include "DrawData.h"

//ShaderProgram* bs();

class Brush : public GameObject{
public:
  Brush() {}
  Brush(std::vector<Vertex> v, Texture diff, Texture spec, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
  ~Brush();
  void draw(ShaderProgram* shp);
  //static ShaderProgram brushShader;
private:
  glm::vec3 position, rotation, scalefactor;
  glm::mat4 model;
  //AABB bb;
  bool updateModel;
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  Texture diffuse, specular;
  std::vector<GLuint> indices;
};

class GameMap{
public:
  GameMap(void (*func)(GameMap* m)) {func(this);}
  GLuint matrices, lights;
  Camera cam;
  ShaderProgram* shaders;
  int shadernum;
  Entity* ents;
  int entnum;
  Brush* brushes;
  int brushnum;
};

#endif
