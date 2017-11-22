#ifndef BRUSH_H
#define BRUSH_H

#include "Base.h"
#include "Shader.h"
#include "GameObject.h"
#include "DrawData.h"

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
  glm::mat3 normalMat;
  //AABB bb;
  bool updateModel;
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  Texture diffuse, specular;
  std::vector<GLuint> indices;
};

#endif
