#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model{
public:
  Model(char* path);
  void draw(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
private:
  glm::mat4 model;
  std::vector<Mesh> meshes;
  std::string directory;
  void processNode(aiNode *node, const aiScene *scene);
  void loadMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
