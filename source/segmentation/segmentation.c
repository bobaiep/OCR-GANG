#include "segmentation.h"
#include "../sdl/our_sdl.h"

void DrawLines(SDL_Surface *image) { 
    Uint32 pixel;
    Uint8 red;

    int isEmpty;

    for (int i = 0; i < image->h; i++) {
        isEmpty = 1;

        for (int j = 0; j < image->w; j++) {
            pixel = get_pixel(image, j, i);
            red = getRed(pixel, image->format);

            if (red == 0)
                isEmpty = 0;
        }

        if (isEmpty) {
            for (int j = 0; j < image->w; j++) {
                put_pixel(image, j, i, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int NumberOfBlocs(SDL_Surface *image) {
    Uint32 pixel;
    Uint8 red;

    int BlocCount = 0;
    int isBloc;
    int blocYmax;

    for (int i = 0; i < image->h; i++) {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);

        if (red == 0 || red == 255) {
            isBloc = 1;
            blocYmax = i;

            while (isBloc && blocYmax < image->h) {
                blocYmax++;

                pixel = get_pixel(image, 0, blocYmax);
                red = getRed(pixel, image->format);

                if (red == 128)
                    isBloc = 0;
            }

            BlocCount++;
            i = blocYmax;
        }
    }
    return BlocCount;
}


void DivideIntoBlocs(SDL_Surface *image, SDL_Surface **blocs, SDL_Surface ***letters) {

    Uint32 pixel;
    Uint8 red;

    SDL_Rect bloc;
    int blocCount = 0;
    int isBloc;
    int blocYmin;
    int blocYmax;

    SDL_Rect letter;
    int isLetter;
    int letterXmin;
    int letterXmax;

    for (int i = 0; i < image->h; i++) {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);

        if (red == 0 || red == 255) {
            isBloc = 1;
            blocYmin = i;
            blocYmax = i;

            while (isBloc && blocYmax < image->h) {
                blocYmax++;

                pixel = get_pixel(image, 0, blocYmax);
                red = getRed(pixel, image->format);

                if (red == 128)
                    isBloc = 0;
            }

            bloc.x = 0;
            bloc.y = blocYmin;
            bloc.w = image->w;
            bloc.h = blocYmax - blocYmin;

            SDL_UnlockSurface(image);
            blocs[blocCount] = SDL_CreateRGBSurface(SDL_HWSURFACE, bloc.w, bloc.h, 32, 0, 0, 0, 0);
            SDL_BlitSurface(image, &bloc, blocs[blocCount], NULL);
            SDL_LockSurface(image);


            /*Start of DivideIntoLetters*/

            DrawColumns(blocs[blocCount]);
            int LettersNumber = NumberOfLetters(blocs[blocCount]);
            letters[blocCount] = malloc(sizeof(SDL_Surface**) * LettersNumber);
            int LetterCount = 0;

            for (int i = 0; i < blocs[blocCount]->w; i++) {

                pixel = get_pixel(blocs[blocCount], i,0);
                red = getRed(pixel, blocs[blocCount]->format);

                if (red == 0 || red == 255) {
                    isLetter = 1;
                    letterXmin = i;
                    letterXmax = i;

                    while (isLetter && letterXmax < blocs[blocCount]->w) {
                        letterXmax++;

                        pixel = get_pixel(blocs[blocCount], letterXmax,0);
                        red = getRed(pixel, blocs[blocCount]->format);

                        if (red == 128)
                            isLetter = 0;
                    }

                    letter.x = letterXmin;
                    letter.y = 0;
                    letter.w = letterXmax - letterXmin;
                    letter.h = blocs[blocCount]->h;

                    SDL_UnlockSurface(blocs[blocCount]);
                    letters[blocCount][LetterCount] = SDL_CreateRGBSurface(SDL_HWSURFACE, letter.w, letter.h, 32, 0, 0, 0, 0);
                    SDL_BlitSurface(blocs[blocCount], &letter, letters[blocCount][LetterCount], NULL);
                    SDL_LockSurface(blocs[blocCount]);

                    LetterCount++;
                    i = letterXmax;
                }
            }

            blocCount++;
            i = blocYmax;
        }
    }
}


void DrawColumns(SDL_Surface *image) {
    Uint32 pixel;
    Uint8 red;

    int isEmpty;

    for (int i = 0; i < image->w; i++) {
        isEmpty = 1;

        for (int j = 0; j < image->h; j++) {
            pixel = get_pixel(image, i, j);
            red = getRed(pixel, image->format);

            if (red == 0)
                isEmpty = 0;
        }

        if (isEmpty) {
            for (int j = 0; j < image->h; j++) {
                put_pixel(image, i, j, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int NumberOfLetters(SDL_Surface *bloc) {
    Uint32 pixel;
    Uint8 red;

    int letterCount = 0;
    int isLetter;
    int LetterXmax;

    for (int i = 0; i < bloc->w; i++) {
        pixel = get_pixel(bloc, i,0);
        red = getRed(pixel, bloc->format);

        if (red == 0 || red == 255) {
            isLetter = 1;
            LetterXmax = i;

            while (isLetter && LetterXmax < bloc->w) {
                LetterXmax++;

                pixel = get_pixel(bloc, LetterXmax, 0);
                red = getRed(pixel, bloc->format);

                if (red == 128)
                    isLetter = 0;
            }

            letterCount++;
            i = LetterXmax;
        }
    }
    return letterCount;
}
