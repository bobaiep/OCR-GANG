#ifndef NN_H_
#define NN_H_


struct network {
  size_t number_of_inputs;
  size_t number_of_hidden_nodes ;
  size_t number_of_outputs ;
  double *input_layer;

  double *hidden_layer;
  double *delta_hidden;
  double *hidden_layer_bias;
  double *hidden_weights;
  double *delta_hidden_weights;

  double *output_layer;
  double *delta_output;
  double *output_layer_bias;
  double *output_weights;
  double *delta_output_weights;

  double eta;
  double alpha;
  double *goal;
};

// Return a pointer to the network object
struct network* InitializeNetwork(double i, double h, double o);

// Initialize the network with random values
void initialization(struct network *net);

// Forward Pass algorithm to obtain a answer out of the neural network
void forward_pass(struct network *net);

// Back Propagation algorithm to compute the errors of the neural network
void back_propagation(struct network *net);

// Update the weights of the neural network
void updateweights(struct network *net);

// Update the biases of the neural network
void UpdateBiases(struct network *net);

#endif
