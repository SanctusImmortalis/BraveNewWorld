#ifndef ENTITY_H
#define ENTITY_H

#include "Base.h"
#include "Brush.h"
#include "Model.h"
#include "GameObject.h"

class Entity:public GameObject{
  public:
    Entity() {}
    Entity(Model* m, ShaderProgram** p, Texture td, Texture ts, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    void setShaders(size_t num1, size_t num2, size_t num3) {this->activeShaders[0] = num1;this->activeShaders[1] = num2;this->activeShaders[2] = num3;}
    void setActiveShaders(size_t num) {this->shaderNum = num;}
    //ShaderProgram* getActiveShader() { return this->prog[this->activeShader];}
    ~Entity();
    void draw();
    Brush* br;
  private:
    size_t activeShaders[3];
    size_t shaderNum;
    ShaderProgram** prog;

};


#endif
