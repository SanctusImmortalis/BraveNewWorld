#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "Mesh.h"
#include "AABB.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model{
public:
  Model(const char* path, ShaderProgram** p);
  void draw(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool updateModel);
  void setActiveShader(size_t num);
  AABB genBoundingBox();

  glm::mat4 model;
  glm::mat3 normalMat;
  size_t activeShader;
  ShaderProgram** prog;
  std::vector<Mesh*> meshes;
  std::string directory;
  void processNode(aiNode *node, const aiScene *scene);
  Mesh* loadMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
