#include <stdio.h> 
#include <stdlib.h> 

float expo(float x) // Self explanitory, just in case cannot use math.h
{ 
    float sum = 1.0f;
    int n = 150; // Arbitrary
    for (int i = n - 1; i > 0; --i )
    {
        sum = 1 + x * sum / i; // Compute by using Taylor's formula
    }
    return sum; 
}

// Activation function and its derivative

double sigmoid(double x)
{
    return 1 / (1 + expo(-x)); 
    // Will be used to adjust the activation of hiddenlayer and outputlayer nodes
}
double dSigmoid(double x)
{ 
    return x * (1 - x); 
    // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
}

// Init all weights and biases between 0.0 and 1.0
double init_weight() 
{ 
    return ((double)rand())/((double)RAND_MAX+1);
}

