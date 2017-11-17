#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale){
  this->vertices = v;
  this->textures = t;
  this->indices = i;
  this->customShader = false;

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
}

void Mesh::draw(glm::mat4 model){
  ShaderProgram* shp = &(Mesh::entityShader);
  if(this->customShader){
    shp = &(this->prog);
  }
  shp->use();
  GLint modelUniform = shp->getUniform("model");
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

  GLint texUniform = 0;
  unsigned int nrDiff = 0, nrSpec = 0;
  for(int i = 0; (i<(this->textures).size())&&(i<16);i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, (this->textures)[i].texHnd);
    if((this->textures)[i].isDiffuse){
      nrDiff += 1;
      texUniform = shp->getUniform(("material.diffuse" + std::to_string(nrDiff)).c_str());
    }else{
      nrSpec += 1;
      texUniform = shp->getUniform(("material.specular" + std::to_string(nrSpec)).c_str());
    }
    if(texUniform<0) break;
    glUniform1i(i);
  }

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, (this->indices).size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}


void enableCustomShader(){
  this->customShader = true;
}

void disableCustomShader(){
  this->customShader = false;
}

void toggleCustomShader(){
  this->customShader = !(this->customShader);
}
