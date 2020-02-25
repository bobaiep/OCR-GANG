// File for Sigmoid functions
// Used for tweaking the weigth of each node in the neural network

#include <math.h> // Be Careful to test this !!!

// Activation function and its derivative

double sigmoid(double x){
     return 1 / (1 + exp(-x)); // Will be used to adjust the activation of hiddenlayer and outputlayer nodes
     }
double dSigmoid(double x) { 
    return x * (1 - x); // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
    }
float exponential(int n, float x) // Self explanitory, just in case cannot use math.h
{ 
    float sum = 1.0f;
    for (int i = n - 1; i > 0; --i ) 
        sum = 1 + x * sum / i; // Compute by using Taylor's formula
    return sum; 
}
// Init all weights and biases between 0.0 and 1.0
double init_weight() { 
    return ((double)rand())/((double)RAND_MAX); // RAND_MAX is a constant whose default value may be at least 32767
    }