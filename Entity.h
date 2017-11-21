#ifndef ENTITY_H
#define ENTITY_H

#include "Base.h"
#include "Model.h"
#include "GameObject.h"

class Entity:public GameObject{
  public:
    Entity() {}
    Entity(Model* m, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    ~Entity();
    void draw();

  private:
    Model* Mod;
    glm::vec3 Pos;
    glm::vec3 Rot;
    glm::vec3 Scale;
    bool updateModel;

};


#endif
