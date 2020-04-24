#ifndef NETWORK_H
#define NETWORK_H


struct network {
    size_t nbInput;
    size_t nbHidden;
    size_t nbOutput;

    double *input;
    struct neurons *hidden;
    struct neurons *output;

    double MaxErrorRate;
    double ErrorRate;
    double eta;
    double alpha;
};

struct neurons
{
    double activation;
    double weight;
    double biais;
    double delta;
    double deltaweight;
};

// Initialize a new neural network
struct network* InitializeNetwork();
struct neurons* InitializeLayer(size_t nb);
struct neurons InitializeNeurons();

void ForwardPass(struct network *net);
void BackwardPropagation(struct network *net);
void UpdateWeightsBiases(struct network *net);
#endif