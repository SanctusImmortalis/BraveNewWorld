#ifndef SHADER_H
#define SHADER_H

#include "Base.h"

class Shader{
public:
  Shader(char shaderType, bool fileSource, const char* shaderSource);
  ~Shader();
  void attach(GLuint shprog);
  char getType();
  void getInfoLog(char* buf) {glGetShaderInfoLog(this->shHnd, 512, NULL, buf);}
  GLint status();
private:
  GLuint shHnd;
  char type;
  GLint stat;
};

class ShaderProgram{
public:
  ShaderProgram() {}
  ShaderProgram(Shader* vs, Shader* gs, Shader* fs);
  ~ShaderProgram();
  void use();
  GLint getUniform(const char* name);
  void setBlockIndex(const char* name, GLuint index);
  GLint status();
private:
  GLuint spHnd;
  GLint stat;
};

#endif
