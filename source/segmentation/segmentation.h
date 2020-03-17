#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

void DrawLines(SDL_Surface *image);
int NumberOfBlocs(SDL_Surface *image);
void DivideIntoBlocs(SDL_Surface *image,SDL_Surface **blocs, SDL_Surface ***letters);
void DrawColumns(SDL_Surface *image);
int NumberOfLetters(SDL_Surface *image);

#endif