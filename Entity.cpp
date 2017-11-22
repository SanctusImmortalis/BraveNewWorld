#include "Entity.h"

Entity::Entity(Model* m, ShaderProgram** p, Texture td, Texture ts, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  /*this->position = pos;
  this->rotation = rot;
  this->scalefactor = scale;
  this->updateModel = true;*/
  this->activeShaders[0] = 0;
  this->activeShaders[1] = 0;
  this->activeShaders[2] = 0;
  this->shaderNum = 1;
  this->prog = p;

  std::vector<Vertex> v;
  std::vector<GLuint> i;
  for(int j=0;j<(m->meshes.size());j++){
    v.insert(v.end(), m->meshes[j]->vertices.begin(), m->meshes[j]->vertices.end());
    i.insert(i.end(), m->meshes[j]->indices.begin(), m->meshes[j]->indices.end());
  }
  br = new Brush(v, td, ts, i, pos, rot, scale);
}

Entity::~Entity(){
}

void Entity::draw(){
  for(int i=0;i<shaderNum && i<3;i++)
    this->br->draw(this->prog[this->activeShaders[i]]);
}
