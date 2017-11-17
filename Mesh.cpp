#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> v, vector<Texture> t, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  this->vertices = v;
  this->textures = t;
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
}

void Mesh::draw(glm::mat4 model){
  ShaderProgram* shp = &(Mesh::entityShader);
  if(this->customShader){
    shp = &(this->prog);
  }
  shp->use();
  GLint modelUniform = shp->getUniform("model");
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, (this->specular).texHnd);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, (this->diffuse).texHnd);

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, (this->indices).size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
