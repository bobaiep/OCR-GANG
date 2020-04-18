#ifndef NETWORK_H
#define NETWORK_H


struct network {
    size_t nbInput;
    size_t nbHidden;
    size_t nbOutput;

    struct layer *input;
    struct layer *hidden;
    struct layer *output;

    double MaxErrorRate;
    double ErrorRate;
    double eta;
    double alpha;
    char alphabet[52];
};

struct layer
{
    struct neurons *data;  
};

struct neurons
{
    double activation;
    double weight;
    double biais;
    struct neurons *next;    
};

// Initialize a new neural network
struct network* InitializeNetwork();
struct layer* InitializeLayer(struct network *net);
struct neurons* InitializeNeurons();

void ForwardPass(struct network *net);
void BackwardPropagation(struct network *net);
void UpdateWeights(struct network *net);
void UpdateBiases(struct network *net);

#endif