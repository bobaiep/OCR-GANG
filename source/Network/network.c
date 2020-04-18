#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "network.h"
#include "tools.h" 

int main() {
    struct network* net = InitializeNetwork();
    printf("initialization finished\n");
    free(net);
}

//Intialize a new network
struct network* InitializeNetwork(){

    struct network *network = malloc(sizeof(struct network));
    if (network == NULL){
        errx(1, "Not enough memory!");
    }
    /*network -> alphabet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',\
                            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};*/
    network -> nbInput = 28*28; // Dimension des images données par la segmentation, a voir avec Marius
    network -> nbHidden = 20; // Arbitraire clairement
    network -> nbOutput = 52; // nb de lettres dans l'alphabet *2
    network -> input = InitializeLayer(network -> nbInput);
    network -> hidden = InitializeLayer(network -> nbHidden);
    network -> output = InitializeLayer(network -> nbOutput);
    if (network-> input == NULL || network->hidden == NULL || network -> output == NULL){
        errx(1, "Not enough memory!");
    }
    network -> ErrorRate = 0.0;
    network -> MaxErrorRate = 0.0;
    network -> eta = 0.5;
    network -> alpha = 0.9;
    return network;

}

//Intialize a new network
struct layer* InitializeLayer(size_t nb){
    struct layer *layer = calloc(nb, sizeof(struct layer));
    if (layer == NULL){
        errx(1, "Not enough memory!");
    }
    for (size_t i = 0 ; i < nb ; i++)
    {
        layer -> data = InitializeNeurons();
    }
    return layer;
}

//Intialize a new network
struct neurons* InitializeNeurons(){
    struct neurons *neuron = malloc(sizeof(struct neurons));
    if (neuron == NULL){
        errx(1, "Not enough memory!");
    }
    neuron -> activation = init_weight(); //Fonction random()
    neuron -> weight = init_weight();
    neuron -> biais = init_weight();
    neuron -> next = NULL;
    return neuron;
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
