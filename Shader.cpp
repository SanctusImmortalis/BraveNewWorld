#include "Shader.h"

Shader::Shader(char shaderType, bool fileSource, char* shaderSource){
  this->stat = 1;
  switch(shaderType){
    case 0:
      this->shHnd = glCreateShader(GL_VERTEX_SHADER);
      break;
    case 1:
      this->shHnd = glCreateShader(GL_GEOMETRY_SHADER);
      break;
    case 2:
      this->shHnd = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      this->stat = -1;
  }

  if(this->stat >= 0){

    if(fileSource){
      std::ifstream fileInput;
      std::stringstream strm;
      fileInput.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      try{
        fileInput.open(shaderSource);
        strm << fileInput.rdbuf();
        fileInput.close();
        shaderSource = strm.str().c_str()
      }catch(std::ifstream::failure e){
        std::cerr << "Couldn't read file" << std::endl;
      }
    }

    glShaderSource(this->shHnd, 1, &(shaderSource), NULL);

    glCompileShader(this->shHnd);

    glGetShaderiv(this->shHnd, GL_COMPILE_STATUS, &(this->stat));
  }
}

Shader::~Shader(){
  glDeleteShader(this->shHnd);
}

void Shader::attach(GLuint shprog){
  glAttachShader(shprog, this->shHnd);
}

GLint Shader::status(){
  return this->stat;
}
