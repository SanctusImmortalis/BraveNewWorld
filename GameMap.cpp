#include "Game.h"
#include "GameMap.h"
#include "ImageLoader.h"

GameMap* currentMap = NULL;

Brush::Brush(std::vector<Vertex> v, Texture diff, Texture spec, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  this->vertices = v;
  this->diffuse = diff;
  this->specular = spec;
  this->indices = i;
  this->position = pos;
  this->rotation = rot;
  this->scalefactor = scale;
  this->model = glm::mat4(1.0f);
  this->updateModel = true;

  glGenVertexArrays(1, &(this->VAO));
  glGenBuffers(1, &(this->VBO));
  glGenBuffers(1, &(this->EBO));

  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, (this->vertices).size() * sizeof(Vertex), &((this->vertices)[0]), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->indices).size() * sizeof(GLuint), &((this->indices)[0]), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  int w, h, c;
  unsigned char* img = loadImg(diff.path.c_str(), &w, &h, &c);
  glGenTextures(1, &(diff.texHnd));
  glBindTexture(GL_TEXTURE_2D, diff.texHnd);
  switch(c){
    case 1:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, img);
    break;
    case 3:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    break;
    case 4:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    break;
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  freeImg(img);

  img = loadImg(spec.path.c_str(), &w, &h, &c);
  glGenTextures(1, &(spec.texHnd));
  glBindTexture(GL_TEXTURE_2D, spec.texHnd);
  switch(c){
    case 1:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, img);
    break;
    case 3:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    break;
    case 4:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    break;
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  freeImg(img);

  glBindTexture(GL_TEXTURE_2D, 0);
}

Brush::~Brush(){
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
}

void Brush::draw(){
  if(this->updateModel){
    this->model = glm::mat4(1.0f);
    this->model = glm::scale(glm::rotate(glm::translate(this->model, this->position), glm::radians(180.0f), this->rotation), this->scalefactor);
    this->updateModel = false;
  }
  ShaderProgram* shp = &(currentMap->brushShader);
  shp->use();
  GLint modelUniform = shp->getUniform("model");
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(this->model));

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, (this->specular).texHnd);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, (this->diffuse).texHnd);

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, (this->indices).size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
