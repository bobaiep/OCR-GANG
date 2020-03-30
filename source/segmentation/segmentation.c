#include "segmentation.h"
#include "../sdl/our_sdl.h"
#include <stdio.h>

void DrawRedLines(SDL_Surface *image) { 
    Uint32 pixel;
    Uint8 red;
    char boo; //boo is boolean
    for (int i = 0; i < image->h; i++) {
        boo = 1;
        for (int j = 0; j < image->w; j++) {
            pixel = get_pixel(image, j, i);
            red = getRed(pixel, image->format);

            if (red == 0)
                boo = 0;
        }
        if (boo) {
            for (int j = 0; j < image->w; j++) {
                put_pixel(image, j, i, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int CountBlocs(SDL_Surface *image) {
    Uint32 pixel;
    Uint8 red;
    int Count = 0; //Count each bloc in image
    char boo; //boo is boolean
    int Ymax;
    for (int i = 0; i < image->h; i++) {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);
        if (red == 0 || red == 255) {
            boo = 1;
            Ymax = i;
            while (boo && Ymax < image->h) {
                Ymax++;
                pixel = get_pixel(image, 0, Ymax);
                red = getRed(pixel, image->format);
                if (red == 128)
                    boo = 0;
            }
            Count++;
            i = Ymax;
        }
    }
    return Count;
}

void DivideIntoBlocs(SDL_Surface *image, SDL_Surface **blocs, SDL_Surface ***chars) {
    Uint32 pixel;
    Uint8 red;
    SDL_Rect bloc;
    int Count = 0;
    char boo;
    int Ymin;
    int Ymax;
    SDL_Rect chr;
    char chrBoo;
    int Xmin;
    int Xmax;
    for (int i = 0; i < image->h; i++) {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);
        if (red == 0 || red == 255) {
            boo = 1;
            Ymin = i;
            Ymax = i;
            while (boo && Ymax < image->h) {
                Ymax++;
                pixel = get_pixel(image, 0, Ymax);
                red = getRed(pixel, image->format);
                if (red == 128)
                    boo = 0;
            }
            bloc.x = 0;
            bloc.y = Ymin;
            bloc.w = image->w;
            bloc.h = Ymax - Ymin;
            SDL_UnlockSurface(image);
            blocs[Count] = SDL_CreateRGBSurface(SDL_HWSURFACE, bloc.w, bloc.h, 32, 0, 0, 0, 0);
            SDL_BlitSurface(image, &bloc, blocs[Count], NULL);
            SDL_LockSurface(image);
            /*Start of DivideIntoBlocs*/
            DrawLinesUp(blocs[Count]);
            int chrNumber = CountChars(blocs[Count]);
            chars[Count] = malloc(sizeof(SDL_Surface**) * chrNumber);
            int chrCount = 0;
            for (int i = 0; i < blocs[Count]->w; i++) {
                pixel = get_pixel(blocs[Count], i,0);
                red = getRed(pixel, blocs[Count]->format);
                if (red == 0 || red == 255) {
                    chrBoo = 1;
                    Xmin = i;
                    Xmax = i;
                    while (chrBoo && Xmax < blocs[Count]->w) {
                        Xmax++;
                        pixel = get_pixel(blocs[Count], Xmax,0);
                        red = getRed(pixel, blocs[Count]->format);
                        if (red == 128)
                            chrBoo = 0;
                    }
                    chr.x = Xmin;
                    chr.y = 0;
                    chr.w = Xmax - Xmin;
                    chr.h = blocs[Count]->h;
                    SDL_UnlockSurface(blocs[Count]);
                    chars[Count][chrCount] = SDL_CreateRGBSurface(SDL_HWSURFACE, chr.w, chr.h, 32, 0, 0, 0, 0);
                    SDL_BlitSurface(blocs[Count], &chr, chars[Count][chrCount], NULL);
                    SDL_LockSurface(blocs[Count]);
                    chrCount++;
                    i = Xmax;
                }
            }
            Count++;
            i = Ymax;
        }
    }
}
void DrawLinesUp(SDL_Surface *image) {
    Uint32 pixel;
    Uint8 red;
    char boo;
    for (int i = 0; i < image->w; i++) {
        boo = 1;
        for (int j = 0; j < image->h; j++) {
            pixel = get_pixel(image, i, j);
            red = getRed(pixel, image->format);
            if (red == 0)
                boo = 0;
        }
        if (boo) {
            for (int j = 0; j < image->h; j++) {
                put_pixel(image, i, j, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int CountChars(SDL_Surface *bloc) {
    Uint32 pixel;
    Uint8 red;
    int count = 0;
    char boo;
    int Xmax;
    for (int i = 0; i < bloc->w; i++) {
        pixel = get_pixel(bloc, i,0);
        red = getRed(pixel, bloc->format);
        if (red == 0 || red == 255) {
            boo = 1;
            Xmax = i;
            while (boo && Xmax < bloc->w) {
                Xmax++;
                pixel = get_pixel(bloc, Xmax, 0);
                red = getRed(pixel, bloc->format);
                if (red == 128)
                    boo = 0;
            }
            count++;
            i = Xmax;
        }
    }
    return count;
}
