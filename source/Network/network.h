#ifndef NETWORK_H
#define NETWORK_H


struct network {
    size_t nbInput;
    size_t nbHidden;
    size_t nbOutput;

    struct neurons *input;
    struct neurons *hidden;
    struct neurons *output;

    double MaxErrorRate;
    double ErrorRate;
    double eta;
    double alpha;
    char alphabet[52];
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
struct neurons* InitializeLayer(size_t nb);
struct neurons InitializeNeurons();

void ForwardPass(struct network *net);
void BackwardPropagation(struct network *net);
void UpdateWeights(struct network *net);
void UpdateBiases(struct network *net);

#endif