#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

void DrawRedLines(SDL_Surface *image);
int CountBlocs(SDL_Surface *image);
int SizeOfChar(SDL_Surface *bloc);
int *DivideIntoBlocs(SDL_Surface *image,SDL_Surface **blocs, SDL_Surface ***chars, int Len);
void DrawLinesUp(SDL_Surface *image);
int CountChars(SDL_Surface *image);
int *Resize1(int *mat, int sizeFinalX, int sizeFinalY, int sizeX, int sizeY);
int ImageToMatrix(SDL_Surface ***chars, int ***chars_matrix, int *len, int BlocNumber);
#endif
