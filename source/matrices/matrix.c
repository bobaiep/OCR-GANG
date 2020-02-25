// Below we define the dimensions of the neural network 
// and we allocate the arrays for the layers, biases and weights: 

static const int number_of_inputs = 2;
static const int number_of_hidden_nodes = 2;
static const int number_of_outputs = 1;
double hidden_layer[numHiddenNodes];
double output_layer[numOutputs];
double hidden_layer_bias[numHiddenNodes];
double output_layer_bias[numOutputs];
double hidden_weights[numInputs][numHiddenNodes];
double output_weights[numHiddenNodes][numOutputs];