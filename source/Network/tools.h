#ifndef TOOLS_H
#define TOOLS_H

// Used to adjust the activation of hiddenlayer and outputlayer nodes
double fsigmoid(double x);

// Derivative of the Sigmoid function
// Will be used to compute the weigth of hiddenlayer and outputlayer nodes
double derivatesigmoid(double x);

// Compute the exp of a number
// In case cannot use "math.h"
float exponential(float x);

// Init all weights and biases between 0.0 and 1.0
// No arguments required
double initneuron();

size_t PosAnswer(struct network *net);
char RetrieveChar(size_t val);
#endif