#ifndef SHADER_H
#define SHADER_H

#include "Base.h"

class Shader{
public:
  Shader(char shaderType, bool fileSource, void* shaderSource);
  ~Shader();
  void attach(GLuint shprog);
  GLint status();
private:
  GLuint shHnd;
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
