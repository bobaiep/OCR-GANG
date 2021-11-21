#include "segmentation.h"

#include <stdio.h>

#include "../sdl/our_sdl.h"
#include "err.h"

void DrawRedLines(SDL_Surface *image)
{
    Uint32 pixel;
    Uint8 red;
    char boo; // boo is boolean
    for (int i = 0; i < image->h; i++)
    {
        boo = 1;
        for (int j = 0; j < image->w; j++)
        {
            pixel = get_pixel(image, j, i);
            red = getRed(pixel, image->format);

            if (red == 0)
                boo = 0;
        }
        if (boo)
        {
            for (int j = 0; j < image->w; j++)
            {
                put_pixel(image, j, i, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int CountBlocs(SDL_Surface *image)
{
    Uint32 pixel;
    Uint8 red;
    int Count = 0; // Count each bloc in image
    char boo; // boo is boolean
    int Ymax;
    for (int i = 0; i < image->h; i++)
    {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);
        if (red == 0 || red == 255)
        {
            boo = 1;
            Ymax = i;
            while (boo && Ymax < image->h)
            {
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

int SizeOfChar(SDL_Surface *bloc)
{
    Uint32 pixel;
    Uint8 red;
    int charSize = 20;
    char boo;
    int charXmax;
    int charXmin;
    for (int i = 0; i < bloc->w; i++)
    {
        pixel = get_pixel(bloc, i, 0);
        red = getRed(pixel, bloc->format);
        if (red == 0 || red == 255)
        {
            boo = 1;
            charXmin = i;
            charXmax = i;
            while (boo && charXmax < bloc->w)
            {
                charXmax++;
                pixel = get_pixel(bloc, charXmax, 0);
                red = getRed(pixel, bloc->format);
                if (red == 128)
                    boo = 0;
            }
            charSize = (charSize + charXmax - charXmin) / 2;
            i = charXmax;
        }
    }
    return charSize;
}

int *DivideIntoBlocs(SDL_Surface *image, SDL_Surface **blocs,
                     SDL_Surface ***chars, int Len)
{
    Uint32 pixel;
    Uint8 red;
    Uint8 green;
    SDL_Rect bloc;
    int Count = 0;
    char boo;
    int Ymin;
    int Ymax;
    SDL_Rect chr;
    SDL_Rect center;
    char chrBoo;
    int Xmin;
    int Xmax;
    int size;
    int *CharsCount = malloc(sizeof(int) * Len);
    for (int i = 0; i < image->h; i++)
    {
        pixel = get_pixel(image, 0, i);
        red = getRed(pixel, image->format);
        if (red == 0 || red == 255)
        {
            boo = 1;
            Ymin = i;
            Ymax = i;
            while (boo && Ymax < image->h)
            {
                Ymax++;
                pixel = get_pixel(image, 0, Ymax);
                red = getRed(pixel, image->format);
                if (red == 128)
                {
                    boo = 0;
                }
            }
            bloc.x = 0;
            bloc.y = Ymin;
            bloc.w = image->w;
            bloc.h = Ymax - Ymin;
            SDL_UnlockSurface(image);
            blocs[Count] = SDL_CreateRGBSurface(SDL_HWSURFACE, bloc.w, bloc.h,
                                                32, 0, 0, 0, 0);
            SDL_BlitSurface(image, &bloc, blocs[Count], NULL);
            DrawLinesUp(blocs[Count]);
            int CharsNumber = CountChars(blocs[Count]);
            SDL_BlitSurface(blocs[Count], NULL, image, &bloc);
            SDL_LockSurface(image);
            CharsCount[Count] = CharsNumber;
            chars[Count] = malloc(sizeof(SDL_Surface *) * CharsNumber);
            int CharCount = 0;
            for (int i = 0; i < blocs[Count]->w; i++)
            {
                pixel = get_pixel(blocs[Count], i, 0);
                red = getRed(pixel, blocs[Count]->format);
                green = getGreen(pixel, blocs[Count]->format);
                if (green == 128)
                {
                    chars[Count][CharCount] = SDL_CreateRGBSurface(
                        SDL_HWSURFACE, bloc.h, bloc.h, 32, 0, 0, 0, 0);
                    SDL_FillRect(chars[Count][CharCount], 0, -1);
                    CharCount++;
                }
                if (red == 0 || red == 255)
                {
                    chrBoo = 1;
                    Xmin = i;
                    Xmax = i;
                    while (chrBoo && Xmax < blocs[Count]->w)
                    {
                        Xmax++;
                        pixel = get_pixel(blocs[Count], Xmax, 0);
                        red = getRed(pixel, blocs[Count]->format);
                        if (red == 128)
                        {
                            chrBoo = 0;
                        }
                    }
                    chr.x = Xmin;
                    chr.y = 0;
                    chr.w = Xmax - Xmin;
                    chr.h = blocs[Count]->h;
                    size = (chr.h < chr.w ? chr.w : chr.h);
                    center.x = size / 2 - chr.w / 2;
                    center.y = size / 2 - chr.h / 2;
                    SDL_UnlockSurface(blocs[Count]);
                    chars[Count][CharCount] = SDL_CreateRGBSurface(
                        SDL_HWSURFACE, size, size, 32, 0, 0, 0, 0);
                    SDL_FillRect(chars[Count][CharCount], 0, -1);
                    SDL_BlitSurface(blocs[Count], &chr, chars[Count][CharCount],
                                    &center);
                    SDL_LockSurface(blocs[Count]);
                    CharCount++;
                    i = Xmax;
                }
            }
            Count++;
            i = Ymax;
        }
    }
    return CharsCount;
}

void DrawLinesUp(SDL_Surface *image)
{
    Uint32 pixel;
    Uint8 red;
    char boo;
    for (int i = 0; i < image->w; i++)
    {
        boo = 1;
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image, i, j);
            red = getRed(pixel, image->format);
            if (red == 0)
                boo = 0;
        }
        if (boo)
        {
            for (int j = 0; j < image->h; j++)
            {
                put_pixel(image, i, j, SDL_MapRGB(image->format, 128, 0, 0));
            }
        }
    }
}

int CountChars(SDL_Surface *bloc)
{
    Uint32 pixel;
    Uint8 red;
    int Count = 0;
    char boo;
    int Xmax;
    int spaceSize = (SizeOfChar(bloc) / 4) * 3;
    int currentspaceSize = 0;
    char insertspace = 1;
    for (int i = 0; i < bloc->w; i++)
    {
        pixel = get_pixel(bloc, i, 0);
        red = getRed(pixel, bloc->format);
        currentspaceSize++;
        if (red == 0 || red == 255)
        {
            insertspace = 1;
            currentspaceSize = 0;
            boo = 1;
            Xmax = i;
            while (boo && Xmax < bloc->w)
            {
                Xmax++;
                pixel = get_pixel(bloc, Xmax, 0);
                red = getRed(pixel, bloc->format);

                if (red == 128)
                {
                    boo = 0;
                }
            }
            Count++;
            i = Xmax;
        }
        if (insertspace && Count != 0 && currentspaceSize == spaceSize)
        {
            insertspace = 0;
            for (int j = 0; j < bloc->h; j++)
            {
                put_pixel(bloc, i, j, SDL_MapRGB(bloc->format, 128, 128, 0));
            }
            Count++;
        }
    }
    return Count;
}

int *Resize1(int *mat, int sizeFinalX, int sizeFinalY, int sizeX, int sizeY)
{
    double coeffX = ((double)sizeX) / ((double)sizeFinalX);
    double coeffY = ((double)sizeY) / ((double)sizeFinalY);
    int *res = malloc(sizeFinalX * sizeFinalY * sizeof(int));
    for (int i = 0; i < sizeFinalY; i++)
    {
        for (int j = 0; j < sizeFinalX; j++)
        {
            res[i * sizeFinalX + j] = mat[((int)((double)i * coeffY)) * sizeX
                                          + (int)((double)j * coeffX) + 1];
        }
    }
    return res;
}

int ImageToMatrix(SDL_Surface ***chars, int ***chars_matrix, int *len,
                  int BlocNumber)
{
    int count = 0;
    for (int j = 0; j < BlocNumber; ++j)
    {
        for (int i = 0; i < len[j]; ++i)
        {
            count++;
        }
    }
    *chars_matrix = malloc(sizeof(int) * 5000);
    if (chars_matrix == NULL)
    {
        errx(1, "Not Enough Memory !");
    }
    count = 0;
    for (int j = 0; j < BlocNumber; ++j)
    {
        for (int i = 0; i < len[j]; ++i)
        {
            (*chars_matrix)[count] = malloc(
                chars[j][i]->w * chars[j][i]->h * sizeof(int) + sizeof(int));
            (*chars_matrix)[count][0] = chars[j][i]->w * chars[j][i]->h;
            for (int y = 0; y < chars[j][i]->h; y++)
            {
                for (int x = 0; x < chars[j][i]->w; x++)
                {
                    Uint32 pixel = get_pixel(chars[j][i], x, y);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, chars[j][i]->format, &r, &g, &b);
                    (*chars_matrix)[count][y * chars[j][i]->w + x + 1] =
                        r == 255 ? 0 : 1;
                }
            }
            int *char_resized = Resize1((*chars_matrix)[count], 28, 28,
                                        chars[j][i]->w, chars[j][i]->h);
            (*chars_matrix)[count] = char_resized;
            count++;
        }
    }
    return count;
}

void SaveMatrix(int **chars_matrix, char *filename)
{
    FILE *matrix = fopen(filename, "w");
    size_t size = 28;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            fprintf(matrix, "%lf", (double)chars_matrix[0][i * size + j]);
        }
        fprintf(matrix, "\n");
    }
    fclose(matrix);
}
