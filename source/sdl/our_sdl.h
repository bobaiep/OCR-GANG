#ifndef OUR_SDL_H_
#define OUR_SDL_H_

#include <SDL/SDL.h>
#include <stdlib.h>

#include "SDL/SDL_image.h"

void init_sdl(void);
SDL_Surface *load__image(char *path);
SDL_Surface *display_image(SDL_Surface *img);
void wait_for_keypressed(void);
Uint8 getRed(Uint32 pixel, SDL_PixelFormat *fmt);
Uint8 getBlue(Uint32 pixel, SDL_PixelFormat *fmt);
Uint8 getGreen(Uint32 pixel, SDL_PixelFormat *fmt);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface *screen, SDL_Surface *image);

#endif
