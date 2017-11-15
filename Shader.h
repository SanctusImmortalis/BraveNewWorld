#ifndef SHADER_H
#define SHADER_H

#include "Base.h"

class Shader{
public:
  Shader(char shaderType, bool sourceType, void* shaderSource);
  ~Shader();
  void compile();
private:
  char* source;
  GLuint shHnd;
};

class ShaderProgram{
public:
  ShaderProgram(Shader* vs, Shader* gs, Shader* fs);
  ~ShaderProgram();
  void link();
  void use();
private:
  GLuint spHnd;
};

#endif
