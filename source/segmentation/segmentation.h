#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

void DrawRedLines(SDL_Surface *image);
int CountBlocs(SDL_Surface *image);
void DivideIntoBlocs(SDL_Surface *image,SDL_Surface **blocs, SDL_Surface ***chars);
void DrawLinesUp(SDL_Surface *image);
int CountChars(SDL_Surface *image);

#endif