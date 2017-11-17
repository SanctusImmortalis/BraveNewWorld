#ifndef IMG_LDR_H
#define IMG_LDR_H

unsigned char* loadImg(char* path, int* width, int* height, int* nrChannels);
void freeImg(char* ptr);


#endif
