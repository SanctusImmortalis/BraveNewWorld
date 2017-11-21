#include "Mesh.h"

//Shader vsm(0, true, "vertexShaderSource.txt");
//Shader fsm(2, true, "fragmentShaderSource.txt");

Mesh::Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<GLuint> i){
  this->vertices = v;
  this->textures = t;
  this->indices = i;

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

Mesh::~Mesh(){
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
}

void Mesh::draw(glm::mat4 model, ShaderProgram* shp){
  shp->use();
  GLint modelUniform = shp->getUniform("model");
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

  {
    GLint texUniform = 0;
    unsigned int nrDiff = 0, nrSpec = 0;
    for(int i = 0; (i<(this->textures).size())&&(i<16);i++){
      std::stringstream ss;
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, (this->textures)[i].texHnd);
      if((this->textures)[i].isDiffuse){
        nrDiff += 1;
        ss << nrDiff;
        texUniform = shp->getUniform(("material.diffuse" + ss.str()).c_str());
      }else{
        nrSpec += 1;
        ss << nrSpec;
        texUniform = shp->getUniform(("material.specular" + ss.str()).c_str());
      }
      if(texUniform<0) break;
      glUniform1i(texUniform, i);
    }
    texUniform = shp->getUniform("nrDiff");
    glUniform1i(texUniform, nrDiff);
    texUniform = shp->getUniform("nrSpec");
    glUniform1i(texUniform, nrSpec);
  }

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, (this->indices).size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
