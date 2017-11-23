#include "Game.h"
#include "GameMap.h"
#include "ImageLoader.h"


Brush::Brush(std::vector<Vertex> v, Texture diff, Texture spec, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  this->vertices = v;
  this->diffuse = diff;
  this->specular = spec;
  this->indices = i;
  this->position = pos;
  this->rotation = rot;
  this->scalefactor = scale;
  this->model = glm::mat4(1.0f);
  this->normalMat = glm::mat3(1.0f);
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

void Brush::draw(ShaderProgram* shp){
  if(this->updateModel){
    this->model = glm::mat4(1.0f);
    glm::mat4 scale = glm::scale(this->model, this->scalefactor);
    glm::mat4 rot;
    glm::mat4 trans = glm::translate(this->model, this->position);
    {
      glm::vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
      glm::vec4 y_axis(0.0f, 1.0f, 0.0f, 1.0f);
      glm::vec4 z_axis(0.0f, 0.0f, 1.0f, 1.0f);
      glm::mat4 rotz = glm::rotate(this->model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
      y_axis = rotz * y_axis;
      x_axis = rotz * x_axis;
      glm::mat4 roty = glm::rotate(this->model, glm::radians(this->rotation.y), glm::vec3(y_axis.x, y_axis.y, y_axis.z));
      x_axis = roty * x_axis;
      glm::mat4 rotx = glm::rotate(this->model, glm::radians(this->rotation.x), glm::vec3(x_axis.x, x_axis.y, x_axis.z));
      rot = rotx * roty * rotz;
    }
    this->model = trans * rot * scale;
    this->normalMat = glm::mat3(glm::transpose(glm::inverse(this->model)));
    this->updateModel = false;
  }
  glm::mat4 md = this->model;
  glm::mat3 nm = this->normalMat;
  shp->use();
  GLint modelUniform = shp->getUniform("model");
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(md));
  modelUniform = shp->getUniform("normalMat");
  glUniformMatrix3fv(modelUniform, 1, GL_FALSE, glm::value_ptr(nm));
  modelUniform = shp->getUniform("diff");
  glUniform1i(modelUniform, 0);
  modelUniform = shp->getUniform("spec");
  glUniform1i(modelUniform, 1);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, (this->specular).texHnd);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, (this->diffuse).texHnd);

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, (this->indices).size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
