#ifndef TOOLS_H_
#define TOOLS_H_

// Print a progress bar in the terminal
void progressBar(int step,int nb);

// Return the sigmoid function applied to x
double sigmoid(double x);

// Return the derivate of the sigmoid function applied to x
double dSigmoid(double x);

// Return the exponential function applied to x
float expo(float x);

// Return values between 0.0 et 1.0
double init_weight();

// Verify if a file exist
int cfileexists(const char * filename);

// Verify if a file is empty
int fileempty(const char * filename);

// Saves the neural network into a file
void save_network(const char * filename,struct network *network);

// Load the values needed for the neural network if saved before
void load_network(const char * filename,struct network *network);

// Shuffle an array
void shuffle(int *array, size_t n);

// Transform the answer of the neural network into a char
char RetrieveChar(size_t val);

// Search for the answer of the neural network
size_t PosAnswer(struct network *net);

#endif
