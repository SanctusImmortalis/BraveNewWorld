#include "Shader.h"

Shader::Shader(char shaderType, bool fileSource, const char* shaderSource){
  this->stat = GL_TRUE;
  this->type = shaderType;
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
      this->stat = GL_FALSE;
  }

  if(this->stat != GL_FALSE){

    if(fileSource){
      std::ifstream fileInput;
      std::stringstream strm;
      fileInput.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      try{
        fileInput.open(shaderSource);
        strm << fileInput.rdbuf();
        fileInput.close();
        shaderSource = strm.str().c_str();
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

char Shader::getType(){
  return this->type;
}

GLint Shader::status(){
  return this->stat;
}

ShaderProgram::ShaderProgram(Shader* vs, Shader* gs, Shader* fs){
  this->spHnd = glCreateProgram();
  if(vs!=NULL){
    if((vs->status()!=GL_FALSE)&&(vs->getType()==0)){
      vs->attach(this->spHnd);
    }else{
      std::cerr << "Vertex shader invalid" << std::endl;
    }
  }else{
    std::cerr << "No vertex shader" << std::endl;
  }
  if(gs!=NULL){
    if((gs->status()!=GL_FALSE)&&(gs->getType()==1)){
      gs->attach(this->spHnd);
    }else{
      std::cerr << "Geometry shader invalid" << std::endl;
    }
  }
  if(fs!=NULL){
    if((fs->status()!=GL_FALSE)&&(fs->getType()==2)){
      fs->attach(this->spHnd);
    }else{
      std::cerr << "Fragment shader invalid" << std::endl;
    }
  }else{
    std::cerr << "No fragment shader" << std::endl;
  }

  glLinkProgram(this->spHnd);
  glGetProgramiv(this->spHnd, GL_LINK_STATUS, &(this->stat));
}

ShaderProgram::~ShaderProgram(){
  glDeleteProgram(this->spHnd);
}

void ShaderProgram::use(){
  glUseProgram(this->spHnd);
}

void ShaderProgram::setBlockIndex(const char* name, GLuint index){
  glUniformBlockBinding(this->spHnd, glGetUniformBlockIndex(this->spHnd, name), index);
}

GLint ShaderProgram::getUniform(const char* name){
  return glGetUniformLocation(this->spHnd, name);
}

GLint ShaderProgram::status(){
  return this->stat;
}
