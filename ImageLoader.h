#ifndef IMG_LDR_H
#define IMG_LDR_H

unsigned char* loadImg(const char* path, int* width, int* height, int* nrChannels);
void freeImg(unsigned char* ptr);


#endif
