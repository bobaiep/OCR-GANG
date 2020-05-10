#ifndef PROCESS_H_
#define PROCESS_H_

#include "../sdl/our_sdl.h"

SDL_Surface* black_and_white(SDL_Surface* image);
SDL_Surface* resize(SDL_Surface* image,int new_w,int new_h);

#endif