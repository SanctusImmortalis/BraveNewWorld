#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* loadImg(char* path, int* width, int* height, int* nrChannels){
  return stbi_load(path, width, height, nrChannels, 0);
}

void freeImg(char* ptr){
  stbi_image_free(ptr);
}
