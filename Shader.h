#ifndef SHADER_H
#define SHADER_H

#include "Base.h"

class Shader{
public:
  Shader(char shaderType, bool fileSource, const char* shaderSource);
  ~Shader();
  void attach(GLuint shprog);
  char getType();
  GLint status();
private:
  GLuint shHnd;
  char type;
  GLint stat;
};

class ShaderProgram{
public:
  ShaderProgram(Shader* vs, Shader* gs, Shader* fs);
  ~ShaderProgram();
  void use();
  GLint status();
private:
  GLuint spHnd;
  GLint stat;
};

#endif
