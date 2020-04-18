#ifndef TOOLS_H
#define TOOLS_H

// Used to adjust the activation of hiddenlayer and outputlayer nodes
double sigmoid(double x);

// Derivative of the Sigmoid function
// Will be used to compute the weigth of hiddenlayer and outputlayer nodes
double dSigmoid(double x);

// Compute the exp of a number
// In case cannot use "math.h"
float expo(float x);

// Init all weights and biases between 0.0 and 1.0
// No arguments required
double init_weight();

#endif