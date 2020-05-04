#ifndef NN_H_
#define NN_H_


struct network {
  int number_of_inputs;
  int number_of_hidden_nodes ;
  int number_of_outputs ;
  double *hidden_layer;
  double *hidden_layer_bias;
  double *hidden_weights;
  double *output_layer;
  double *output_layer_bias;
  double *output_weights;
  double lr;
};

struct network* InitializeNetwork(double i, double h, double o);

void initialization(struct network *net);

int forward_pass(int x ,struct network *net,int trainingSetOrder[],\
  double training_inputs[]);

void back_propagation(struct network *net, int i,double training_inputs[],double training_outputs[]);

#endif
