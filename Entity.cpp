#include "Entity.h"

Entity::Entity(Model* m, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  this->Mod = m;
  this->Pos = pos;
  this->Rot = rot;
  this->Scale = scale;
  this->updateModel = true;
}

Entity::~Entity(){
}

void Entity::draw(){
  this->Mod->draw(this->Pos, this->Rot, this->Scale, this->updateModel);
  this->updateModel = false;
}
