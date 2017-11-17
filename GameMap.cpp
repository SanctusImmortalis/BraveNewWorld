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

  int w, h, c;
  unsigned char* img = loadImg(diff.path.c_str(), &w, &h, &c);
  glGenTextures(1, &(diff.texHnd));
  glBindTexture(GL_TEXTURE_2D, diff.texHnd);
  switch(c){
    case 1:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_R, GL_UNSIGNED_BYTE, img);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_R, GL_UNSIGNED_BYTE, img);
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

  glBindTexture(GL_TEXTURE_2D, 0);
}
