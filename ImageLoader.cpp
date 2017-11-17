#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* loadImg(const char* path, int* width, int* height, int* nrChannels){
  stbi_set_flip_vertically_on_load(true);
  return stbi_load(path, width, height, nrChannels, 0);
}

void freeImg(unsigned char* ptr){
  stbi_image_free(ptr);
}
