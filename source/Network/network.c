#include <stdio.h>
#include <err.h>
#include "network.h"
#include "tools.h" 

//Intialize a new network
struct network* InitializeNetwork(){

    struct network network = malloc(sizeof(struct network));
    if (network == NULL){
        errx(1, "Not enough memory!");
    }
    network -> alphabet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',\
                            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    network -> nbInput = 28*28; // Dimension des images données par la segmentation, a voir avec Marius
    network -> nbHidden = 20; // Arbitraire clairement
    network ->  = 52; // nb de lettres dans l'alphabet *2
    network -> input = calloc(network->nbInput, sizeof(struct layer));
    network -> hidden = calloc(network->nbHidden, sizeof(struct layer));
    network -> output = calloc(network->nbOutput, sizeof(struct layer));
    if (network-> input == NULL || network->hidden == NULL || network -> output == NULL){
        errx(1, "Not enough memory!");
    }
    network -> ErrorRate = 0.0;
    network -> MaxErrorRate = 0.0;
    network -> eta = 0.5;
    network -> alpha = 0.9;

}

//Intialize a new network
struct layer* InitializeLayer(struct network *net){
    struct layer *layer = malloc(sizeof(struct layer));
    if (layer == NULL){
        errx(1, "Not enough memory!");
    }
    for (size_t i = 0 ; i < net -> nbInput ; i++)
    {
        h -> input[i] = InitializeNeurons();
    }
    for (size_t i = 0 ; i < net -> nbHidden ; i++)
    {
        h -> hidden[i] = InitializeNeurons();
    }
    for (size_t i = 0 ; i < net -> nbOutput ; i++)
    {
        h -> output[i] = InitializeNeurons();
    }
}

//Intialize a new network
struct neurons* InitializeNeurons(){
    struct neurons *neuron = malloc(sizeof(struct neurons));
    if (neuron == NULL){
        errx(1, "Not enough memory!");
    }
    neuron -> activation = ; //Fonction random()
    neuron -> weight = init_weight();
    neuron -> biais = ;
    neuron -> next = NULL;
}

void ForwardPass(struct network *net)
{
  // TODO
}

void BackwardPropagation(struct network *net)
{
  //TODO
}

void UpdateWeights(struct network *net)
{
  //TODO
}

void UpdateBiases(struct network *net)
{
  //TODO
}