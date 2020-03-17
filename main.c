#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "source/sdl/our_sdl.h"
#include "source/segmentation/segmentation.h"
#include "source/process/process.h"
#include "err.h"


int main(int argc, char** argv) {
    if (argc!=2)
    {
        errx(1,"main: Usage: image_path");
    }
    
    init_sdl(); /* Init SDL */

    /* Load Image */
    SDL_Surface* image=load_image(argv[1]);

    SDL_LockSurface(image);
    
    /* Black and White */
    image = black_and_white(image);

    DrawLines(image); //Separates into blocs delimited by red color
    int BlocNumber = NumberOfBlocs(image);
    SDL_Surface ***letters = malloc(sizeof(SDL_Surface**) * BlocNumber);
    SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocNumber);

    DivideIntoBlocs(image,blocs,letters); //Divides image into lines

    char file[12];
    for (int j = 0; j < BlocNumber; ++j) {
        for (int i = 0; i < NumberOfLetters(blocs[j]); ++i) {
            sprintf(file,"%i",j*100 + i);
            strcat(file,".bmp");
            SDL_SaveBMP(letters[j][i], file); //Save blocs in folder
        }
    }

    SDL_UnlockSurface(image);

    SDL_SaveBMP(image,"new.bmp"); //Save image in folder

    /*
    SDL_Surface* screen = display_image(image);

    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);

    SDL_FreeSurface(screen);
    */

    SDL_Quit();
    printf("%i \n",NumberOfLetters(blocs[0]));

    return 0;
}

