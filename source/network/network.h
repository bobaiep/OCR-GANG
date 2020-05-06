#ifndef NN_H_
#define NN_H_


struct network {
  int number_of_inputs;
  int number_of_hidden_nodes ;
  int number_of_outputs ;
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

struct network* InitializeNetwork(double i, double h, double o);

void initialization(struct network *net);

void forward_pass(struct network *net);

void back_propagation(struct network *net);

void updateweightsetbiases(struct network *net);

void InputImage(struct network *net,size_t index,int ***chars_matrix);

#endif
