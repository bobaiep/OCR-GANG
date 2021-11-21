#include "process.h"

#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

SDL_Surface *black_and_white(SDL_Surface *image)
{
    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            /* Extracting color components from a 32-bit color value */
            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            /* Grey level and black and white */
            float average = (r + g + b) / 3; // Niveaux de gris
            Uint8 color = (average >= 180) ? 255 : 0; // Noir ou blanc
            put_pixel(image, i, j,
                      SDL_MapRGB(image->format, color, color, color));
        }
    }
    return image;
}

SDL_Surface *resize(SDL_Surface *image, int new_w, int new_h)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(
        SDL_HWSURFACE, new_w, new_h, image->format->BitsPerPixel, 0, 0, 0, 0);
    SDL_SoftStretch(image, NULL, dest, NULL);
    return dest;
}