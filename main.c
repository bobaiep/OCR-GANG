#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "source/sdl/our_sdl.h"
#include "source/segmentation/segmentation.h"
#include "source/process/process.h"
#include "source/matrix/matrix.h"
#include "source/sigmoid/sigmoid.h"
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
        init_sdl(); /* Init SDL */

        /* Load Image */
        SDL_Surface* image=load_image(argv[2]);

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
                char loc[100] = "img/temp/";
                sprintf(file,"%i",j*100 + i);
                strcat(file,".bmp");
                SDL_SaveBMP(letters[j][i], strcat(loc,file)); //Save blocs in folder
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
    }
    else{
        if(strcmp(argv[1], "--XOR")==0){
            /* Declare all we are gonna need */
            static const int number_of_inputs = 2;
            static const int number_of_hidden_nodes = 2;
            static const int number_of_outputs = 1;
            double hidden_layer[number_of_hidden_nodes];
            double output_layer[number_of_outputs];
            double hidden_layer_bias[number_of_hidden_nodes];
            double output_layer_bias[number_of_outputs];
            double hidden_weights[number_of_inputs*number_of_hidden_nodes];
            double output_weights[number_of_hidden_nodes*number_of_outputs];
            static const int number_training_sets = 4;
            // creating file pointer to work with files
            FILE *result_file;
            // opening file in writing mode
            result_file = fopen("source/Xor/Xor-data.txt", "w");
            // TODO : fill in the training inputs and outputs - not working rn, dont't know why
            // Tried : 
            double training_inputs[] = {0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f};
            double training_outputs[]= {0.0f,1.0f,1.0f,0.0f};
            int trainingSetOrder[] = {0,1,2,3};
            const double lr = 0.1f;

            //TODO : See if the file exist ()
            /* Init all the matrices (suppose that the file is empty)*/
            for (int i=0; i<number_of_inputs; i++)
            {
                for (int j=0; j<number_of_hidden_nodes; j++)
                {
                hidden_weights[i*number_of_hidden_nodes+j] = init_weight();
                }
            }
            for (int i = 0; i < number_of_hidden_nodes; i++)
            {
                hidden_layer_bias[i] = init_weight();
                for (int j = 0; j < number_of_outputs; j++)
                {
                output_weights[i*number_of_outputs+j] = init_weight();
                }
            }
            for (int i = 0; i < number_of_hidden_nodes; i++)
            {
                output_layer_bias[i] = init_weight();
            }
            printf("Finished all initialization !\n");
            printf("Started computing ... \n");
            printf("/ \r");
            for (int n=0; n < 7031; n++)
            {
                shuffle(trainingSetOrder,number_training_sets);
                for (int x=0; x<number_training_sets; x++)
                {
                printf("-- \r");
                /*DONE : Foward pass = actually input some value into
                the neural network and see what we obtain out of it*/
                int i = trainingSetOrder[x]; // Select a random tuple
                for (int j=0; j<number_of_hidden_nodes; j++)
                {
                    double activation=hidden_layer_bias[j];
                    for (int k=0; k<number_of_inputs; k++)
                    {
                    activation+=training_inputs[i*number_of_inputs+k]*hidden_weights[k*number_of_hidden_nodes+j];
                    }
                    hidden_layer[j] = sigmoid(activation);
                }
                printf("'\' \r");
                for (int j=0; j<number_of_outputs; j++)
                {
                    double activation=output_layer_bias[j];
                    for (int k=0; k<number_of_hidden_nodes; k++)
                    {
                    activation+=hidden_layer[k]*output_weights[k*number_of_outputs+j];
                    }
                    output_layer[j] = sigmoid(activation);
                }
                printf("| \r");
                /* DONE : write down the results contained in ouput_layer[0]
                compared to what the results should have been 
                => Write it into a file to see the evolution*/
                fprintf(result_file, "input : %f ^ %f => output = %f , expected : %f\n",training_inputs[i*number_of_inputs],training_inputs[i*number_of_inputs+1],output_layer[0],training_outputs[i*number_of_outputs]);
                // DONE : Back propagation = update the weight according to
                // the result that we should have obtained
                double deltaOutput[number_of_inputs];
                for (int j=0; j<number_of_inputs; j++) {
                    double errorOutput = (training_outputs[i*number_of_outputs+j]-output_layer[j]);
                    deltaOutput[j] = errorOutput*dSigmoid(output_layer[j]);
                }
                printf("/ \r");
                double deltaHidden[number_of_hidden_nodes];
                for (int j=0; j<number_of_hidden_nodes; j++) {
                    double errorHidden = 0.0f;
                    for(int k=0; k<number_of_inputs; k++) {
                        errorHidden+=deltaOutput[k]*output_weights[j*number_of_outputs+k];
                    }
                    deltaHidden[j] = errorHidden*dSigmoid(hidden_layer[j]);
                }
                printf("-- \r");
                for (int j=0; j<number_of_inputs; j++)
                {
                    output_layer_bias[j] += deltaOutput[j]*lr;
                    for (int k=0; k<number_of_hidden_nodes; k++)
                    {
                        output_weights[k+j*number_of_hidden_nodes]+=hidden_layer[k]*deltaOutput[j]*lr;
                    }
                }
                printf("'\' \r");
                // TOFIX : Update the weights of hidden layer - Not working (memory leak) when uncomment 
                for(int w = 0;w < number_of_hidden_nodes;w++)
                {
                    hidden_layer_bias[w] += deltaHidden[w]*lr;
                    for(int k=0; k<number_of_inputs; k++) 
                    {
                        hidden_weights[k*number_of_inputs+w] += lr*training_inputs[i*number_of_inputs+k]* deltaHidden[w];
                    }
                }
                }
            }
            fclose(result_file);
            //TODO : write down the final weight and the biases of the hidden layer and the output layer into Xor-weights.txt
            // creating file pointer to work with files
            printf("Done! \n");
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



