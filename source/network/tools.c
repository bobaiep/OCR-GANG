#include "../network/tools.h"

#include <stdio.h>

#include "../network/network.h"
#include "../process/process.h"
#include "../sdl/our_sdl.h"
#include "../segmentation/segmentation.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define KRED "\x1B[31m"
#define KWHT "\x1B[37m"
#define KGRN "\x1B[32m"

void progressBar(int step, int nb)
{
    printf("\e[?25l");
    int percent = (step * 100) / nb;
    const int pwidth = 72;
    int pos = (step * pwidth) / nb;
    printf("[");
    for (int i = 0; i < pos; i++)
    {
        printf("%c", '=');
    }
    printf("%*c ", pwidth - pos + 1, ']');
    printf(" %3d%%\r", percent);
}

float expo(float x) // Self explanitory, just in case cannot use math.h
{
    float sum = 1.0f;
    int n = 150; // Arbitrary
    for (int i = n - 1; i > 0; --i)
    {
        sum = 1 + x * sum / i; // Compute by using Taylor's formula
    }
    return sum;
}

// Activation function and its derivative

double sigmoid(double x)
{
    return 1 / (1 + expo(-x));
    // Will be used to adjust the activation of hiddenlayer and outputlayer
    // nodes
}
double dSigmoid(double x)
{
    return x * (1 - x);
    // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
}

// Init all weights and biases between 0.0 and 1.0
double init_weight()
{
    return ((double)rand()) / ((double)RAND_MAX + 1);
}

int cfileexists(const char *filename)
{
    /* try to open file to read */
    FILE *file;
    file = fopen(filename, "r");
    if (!file)
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

int fileempty(const char *filename)
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    fseek(fptr, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fptr);
    if (len > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void save_network(const char *filename, struct network *network)
{
    FILE *output = fopen(filename, "w");
    for (int k = 0; k < network->number_of_inputs; k++)
    {
        for (int o = 0; o < network->number_of_hidden_nodes; o++)
        {
            fprintf(output, "%lf %lf\n", network->hidden_layer_bias[o],
                    network->hidden_weights[k * network->number_of_hidden_nodes
                                            + o]);
        }
    }
    for (int i = 0; i < network->number_of_hidden_nodes; i++)
    {
        for (int a = 0; a < network->number_of_outputs; a++)
        {
            fprintf(
                output, "%lf %lf\n", network->output_layer_bias[a],
                network->output_weights[i * network->number_of_outputs + a]);
        }
    }
    fclose(output);
}

void load_network(const char *filename, struct network *network)
{
    FILE *input = fopen(filename, "r");
    for (int k = 0; k < network->number_of_inputs; k++)
    {
        for (int o = 0; o < network->number_of_hidden_nodes; o++)
        {
            fscanf(input, "%lf %lf\n", &network->hidden_layer_bias[o],
                   &network->hidden_weights[k * network->number_of_hidden_nodes
                                            + o]);
        }
    }
    for (int i = 0; i < network->number_of_hidden_nodes; i++)
    {
        for (int a = 0; a < network->number_of_outputs; a++)
        {
            fscanf(
                input, "%lf %lf\n", &network->output_layer_bias[a],
                &network->output_weights[i * network->number_of_outputs + a]);
        }
    }
    fclose(input);
}

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

size_t IndexAnswer(struct network *net)
{
    size_t index = 0;
    for (size_t i = 1; i < (size_t)net->number_of_outputs; i++)
    {
        if (net->output_layer[i] > net->output_layer[index])
        {
            index = i;
        }
    }
    return index;
}
char RetrieveChar(size_t val)
{
    char c;

    if (val <= 25)
    {
        c = val + 65;
    }
    else if (val > 25 && val <= 51)
    {
        c = (val + 97 - 26);
    }
    else if (val > 51 && val <= 61)
    {
        c = val + 48 - 52;
    }
    else
    {
        switch (val)
        {
        case 62:
            c = ';';
            break;
        case 63:
            c = '\'';
            break;
        case 64:
            c = ':';
            break;
        case 65:
            c = '-';
            break;
        case 66:
            c = '.';
            break;
        case 67:
            c = '!';
            break;
        case 68:
            c = '?';
            break;
        case 69:
            c = '(';
            break;
        case 70:
            c = '\"';
            break;
        case 71:
            c = ')';
            break;
        default:
            exit(1);
            break;
        }
    }
    return c;
}

size_t ExpectedPos(char c)
{
    size_t index = (size_t)c;
    if (c >= 'A' && c <= 'Z')
    {
        index -= 65;
    }
    if (c >= 'a' && c <= 'z')
    {
        index -= 71;
    }
    return index;
}

void ExpectedOutput(struct network *network, char c)
{
    if (c >= 'A' && c <= 'Z')
        network->goal[(int)(c)-65] = 1;

    else if (c >= 'a' && c <= 'z')
        network->goal[((int)(c)-97) + 26] = 1;
}

char *updatepath(char *filepath, size_t len, char c, size_t index)
{
    char *newpath = malloc(len * sizeof(char));
    for (size_t i = 0; i < len; i++)
    {
        if (i != 17)
        {
            newpath[i] = filepath[i];
        }
        else
        {
            newpath[i] = c;
        }
    }
    if (c <= 'Z')
    {
        newpath[14] = 'a';
        newpath[15] = 'j';
    }
    else
    {
        newpath[14] = 'i';
        newpath[15] = 'n';
    }
    newpath[18] = (char)(index + 48);
    newpath[23] = '\0';
    return newpath;
}

void PrintState(char expected, char obtained)
{
    printf("Char entered: %c | Char recoginized: %c ", expected, obtained);
    if (expected == obtained)
    {
        printf("=> %sOK%s\n", KGRN, KWHT);
    }
    else
    {
        printf("=> %sKO%s\n", KRED, KWHT);
    }
}

void InputFromTXT(char *filepath, struct network *net)
{
    FILE *file = fopen(filepath, "r");
    size_t size = 28;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            fscanf(file, "%lf", &net->input_layer[i * size + j]);
        }
        fscanf(file, "\n");
    }
    fclose(file);
}

void PrepareTraining()
{
    init_sdl();
    char *filepath = "img/training/maj/A0.png\0";
    char *filematrix = "img/training/maj/A0.txt\0";
    char expected_result[52] = { 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E',
                                 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i',
                                 'J', 'j', 'K', 'k', 'L', 'I', 'M', 'm', 'N',
                                 'n', 'O', 'o', 'P', 'p', 'Q', 'q', 'R', 'r',
                                 'S', 's', 'I', 't', 'U', 'u', 'V', 'v', 'W',
                                 'w', 'X', 'x', 'Y', 'y', 'Z', 'z' };
    int **chars_matrix = NULL;

    int nb = 52;
    for (size_t i = 0; i < (size_t)nb; i++)
    {
        for (size_t index = 0; index < 4; index++)
        {
            filepath = updatepath(filepath, (size_t)strlen(filepath),
                                  expected_result[i], index);
            filematrix = updatepath(filematrix, (size_t)strlen(filepath),
                                    expected_result[i], index);

            SDL_Surface *image = load__image(filepath);
            image = black_and_white(image);
            DrawRedLines(image);
            int BlocCount = CountBlocs(image);
            SDL_Surface ***chars = malloc(sizeof(SDL_Surface **) * BlocCount);
            SDL_Surface **blocs = malloc(sizeof(SDL_Surface *) * BlocCount);
            int *charslen = DivideIntoBlocs(image, blocs, chars, BlocCount);

            for (int j = 0; j < BlocCount; ++j)
            {
                SDL_FreeSurface(blocs[j]);
            }
            ImageToMatrix(chars, &chars_matrix, charslen, BlocCount);
            SaveMatrix(chars_matrix, filematrix);
        }
    }
    free(chars_matrix);
}
