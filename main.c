#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "source/sdl/our_sdl.h"
#include "source/segmentation/segmentation.h"
#include "source/process/process.h"
#include "source/nn/nn.h"
#include "source/nn/tools.h"
#include "err.h"

int main(int argc, char** argv) {
    if (argc<2){
        printf("-----------------------\n");
        printf("Bienvenue dans OCR GANG\n");
        printf("-----------------------\n");
        printf("Arguments :\n");
        printf("    --seg   Montre la segmentation (spécifiez un image path)\n");
        printf("    --XOR   Montre la fonction XOR\n");
        return 0;
    }
    if (strcmp(argv[1], "--seg") == 0){
        /* Init SDL */
        init_sdl();
        /* Load Image */
        SDL_Surface* image = load_image(argv[2]);
        SDL_Surface* screen_surface = display_image(image);
        wait_for_keypressed();
        /* Black and White */
        image = black_and_white(image);
        screen_surface = display_image(image);
        wait_for_keypressed();
        SDL_SaveBMP(image,"binarisation.bmp");
        DrawRedLines(image);
        int BlocCount = CountBlocs(image);
        SDL_Surface ***chars = malloc(sizeof(SDL_Surface**) * BlocCount);
        SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocCount);
        int *charslen = DivideIntoBlocs(image,blocs,chars, BlocCount); //Divides image into lines
        SDL_SaveBMP(image,"segmentation.bmp"); //Save image in folder
        for (int j = 0; j < BlocCount; ++j) {
            SDL_FreeSurface(blocs[j]);
        }
        SDL_Surface* new_image=load_image("segmentation.bmp");
        screen_surface = display_image(new_image);
        wait_for_keypressed();
        int **chars_matrix =  NULL;
        int chars_count = ImageToMatrix(chars,&chars_matrix, charslen, BlocCount);
        SDL_FreeSurface(new_image);
        SDL_FreeSurface(screen_surface);
        SDL_Quit();
        FILE * output = fopen("output.tst","w");
        for (int k = 0; k < chars_count; ++k) {
            //printf("\n \n Letter number : %i \n",k);
            //printf("{");
            int i;
            for (i = 0; i < 900 - 1; ++i) {
                //printf("%i,", letters_matrix[k][i]);
                putc(chars_matrix[k][i] +'0', output);
                if ((i+1)*(i+1) % 30 == 0) {
                    putc('\n',output);
                    //printf("\n");
                }
            }
            putc(chars_matrix[k][i] +'0', output);
            //printf("%i} \n", letters_matrix[k][i]);
            fprintf(output,"\n\n");
        }
        for (int j = 0; j < BlocCount; ++j) {
            for (int i = 0; i < CountChars(blocs[j]); ++i) {
                SDL_FreeSurface(chars[j][i]);
            }
        }
    }
    else{
        if(strcmp(argv[1], "--XOR")==0){
          /*Creation of neural network*/
          struct network *network = InitializeNetwork(2,4,1);

          static const int number_training_sets = 4;
          FILE *result_file;
          result_file = fopen("source/Xor/Xor-data.txt", "w");
          double training_inputs[] = {0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f};
          double training_outputs[]= {0.0f,1.0f,1.0f,0.0f};
          int trainingSetOrder[] = {0,1,2,3};

          printf("Finished all initialization !\n");
          char answer[1];
          printf("Do you want to train the neural network or use it ?\n1 = Train it\n2 = Use it\n");
          fgets(answer,2,stdin);
          if (atoi(&answer[0])== 1)
          {
              printf("Started computing ... \n");
              int nb = 10000;
              int step = 0;
              for (int n=0; n < nb; n++)
              {
                  step++;
                  progressBar(step,nb);
                  shuffle(trainingSetOrder,number_training_sets);
                  for (int x=0; x<number_training_sets; x++)
                  {
                      int index = trainingSetOrder[x];
                      network -> input_layer[0] = training_inputs[2*index];
                      network -> input_layer[1] = training_inputs[2*index+1];
                      network -> goal[0] = training_outputs[index];
                      forward_pass(network);
                      back_propagation(network);
                      updateweights(network);
                      UpdateBiases(network);
                      fprintf(result_file, "input : %f ^ %f => output = %f , expected : %f\n",network->input_layer[0],network->input_layer[1],network->output_layer[0],training_outputs[index]);
                  }
                  fprintf(result_file, "\n");
              }
              printf("\n");
              printf("\e[?25h");
              fclose(result_file);
              //save_weights_bias(number_of_outputs,number_of_hidden_nodes,number_of_inputs,output_layer_bias,output_weights,hidden_layer_bias,hidden_weights);
          }
          else if (atoi(&answer[0])== 2)
          {
            double number1;
            double number2;
            printf("Please input the first number :\n");
            scanf("%lf\n",&number1);
            printf("Please input the second number :\n");
            scanf("%lf\n",&number2);
          }

        }
        else{
            printf("-----------------------\n");
            printf("Bienvenue dans OCR GANG\n");
            printf("-----------------------\n");
            printf("Arguments :\n");
            printf("    --seg   Montre la segmentation (spécifiez un image path)\n");
            printf("    --XOR   Montre la fonction XOR\n");
            return 0;
        }
    }
    return 0;
}
