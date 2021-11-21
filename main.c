#include <stdio.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "err.h"
#include "source/GUI/gui.h"
#include "source/network/network.h"
#include "source/network/tools.h"
#include "source/process/process.h"
#include "source/sdl/our_sdl.h"
#include "source/segmentation/segmentation.h"

#define KRED "\x1B[31m"
#define KWHT "\x1B[37m"
#define UNUSED(x) (void)(x)

void XOR()
{
    /*Creation of neural network*/
    struct network *network =
        InitializeNetwork(2, 4, 1, "source/Xor/xorwb.txt");

    static const int number_training_sets = 4;
    FILE *result_file;
    result_file = fopen("source/Xor/xordata.txt", "w");
    double training_inputs[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    double training_outputs[] = { 0.0f, 1.0f, 1.0f, 0.0f };
    int trainingSetOrder[] = { 0, 1, 2, 3 };

    printf("Finished all initialization !\n");
    char answer[2];
    printf("Do you want to train the neural network or use it ?\n1 = Train "
           "it\n2 = Use it\n");
    if (fgets(answer, 2, stdin) == NULL)
        errx(1, "Error !");
    if (atoi(&answer[0]) == 1)
    {
        printf("Started computing ... \n");
        int nb = 10000;
        int step = 0;
        for (int n = 0; n < nb; n++)
        {
            step++;
            progressBar(step, nb);
            shuffle(trainingSetOrder, number_training_sets);
            for (int x = 0; x < number_training_sets; x++)
            {
                int index = trainingSetOrder[x];
                network->input_layer[0] = training_inputs[2 * index];
                network->input_layer[1] = training_inputs[2 * index + 1];
                network->goal[0] = training_outputs[index];
                forward_pass(network);
                back_propagation(network);
                updateweightsetbiases(network);
                fprintf(result_file,
                        "input : %f ^ %f => output = %f , expected : %f\n",
                        network->input_layer[0], network->input_layer[1],
                        network->output_layer[0], training_outputs[index]);
            }
            fprintf(result_file, "\n");
        }
        printf("\n");
        printf("\e[?25h");
        fclose(result_file);
        save_network("source/Xor/xorwb.txt", network);
        free(network);
    }
    else if (atoi(&answer[0]) == 2)
    {
        printf("%sBUGGY RIGHT NOW !%s\n", KRED, KWHT);
        printf("Please input the first number :\n");
        (void)scanf("%lf\n", &network->input_layer[0]);
        printf("Please input the second number :\n");
        (void)scanf("%lf\n", &network->input_layer[1]);
        forward_pass(network);
        printf("The neural network returned : %f\n", network->output_layer[0]);
    }
}

void StartOCR(char *filepath)
{
    struct network *network =
        InitializeNetwork(28 * 28, 20, 52, "source/OCR/ocrwb.txt");
    init_sdl();
    SDL_Surface *image = load__image(filepath);
    image = black_and_white(image);
    DrawRedLines(image);
    int BlocCount = CountBlocs(image);
    SDL_Surface ***chars = malloc(sizeof(SDL_Surface **) * BlocCount);
    SDL_Surface **blocs = malloc(sizeof(SDL_Surface *) * BlocCount);
    int *charslen = DivideIntoBlocs(image, blocs, chars, BlocCount);
    SDL_SaveBMP(image, "segmentation.bmp");
    for (int j = 0; j < BlocCount; ++j)
    {
        SDL_FreeSurface(blocs[j]);
    }
    int **chars_matrix = NULL;
    int chars_count = ImageToMatrix(chars, &chars_matrix, charslen, BlocCount);

    char *result = calloc(chars_count, sizeof(char));

    for (size_t index = 0; index < (size_t)chars_count; index++)
    {
        int is_espace = InputImage(network, index, &chars_matrix);
        if (!is_espace)
        {
            forward_pass(network);
            size_t index_answer = IndexAnswer(network);
            result[index] = RetrieveChar(index_answer);
        }
        else
        {
            result[index] = ' ';
        }
    }
    SDL_Quit();
    free(network);
    printf("%s\n", result);
}

void TNeuralNetwork()
{
    struct network *network =
        InitializeNetwork(28 * 28, 20, 52, "source/OCR/ocrwb.txt");
    char *filepath = "img/training/maj/A0.txt\0";
    char expected_result[52] = { 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E',
                                 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i',
                                 'J', 'j', 'K', 'k', 'L', 'I', 'M', 'm', 'N',
                                 'n', 'O', 'o', 'P', 'p', 'Q', 'q', 'R', 'r',
                                 'S', 's', 'I', 't', 'U', 'u', 'V', 'v', 'W',
                                 'w', 'X', 'x', 'Y', 'y', 'Z', 'z' };
    int nb = 2500;
    for (size_t number = 0; number < (size_t)nb; number++)
    {
        for (size_t i = 0; i < 52; i++)
        {
            ExpectedOutput(network, expected_result[i]);
            InputFromTXT(filepath, network);
            forward_pass(network);
            PrintState(expected_result[i], RetrieveChar(IndexAnswer(network)));
            back_propagation(network);
            updateweightsetbiases(network);
        }
    }
    save_network("source/OCR/ocrwb.txt", network);
    free(network);
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        InitGUI(argc, argv);
        return 0;
    }
    else if (strcmp(argv[1], "--XOR") == 0)
    {
        XOR();
    }
    else if (strcmp(argv[1], "--OCR") == 0 && argc == 3)
    {
        if (cfileexists(argv[2]))
        {
            PrepareTraining();
            TNeuralNetwork();
            StartOCR(argv[2]);
        }
        else
        {
            printf("There is no such image, please specify a correct path.\n");
        }
    }

    else if (strcmp(argv[1], "--train") == 0)
    {
        PrepareTraining();
        TNeuralNetwork();
    }

    else
    {
        printf("-----------------------\n");
        printf("Bienvenue dans OCR GANG\n");
        printf("-----------------------\n");
        printf("Arguments :\n");
        printf("    (Aucun) Lance l'interface utilisateur (GUI)\n");
        printf("    --train Lance l'entrainement du réseau de neurones\n");
        printf("    --OCR   Lance l'OCR (spécifiez un image path)\n");
        printf("    --XOR   Montre la fonction XOR\n");
    }
    return 0;
}
