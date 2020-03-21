#ifndef SIGMOID_H_
#define SIGMOID_H_
void progressBar(int step,int nb);
double sigmoid(double x);
double dSigmoid(double x);
float expo(float x);
double init_weight();
void save_weights_bias(int number_of_outputs,int number_of_hidden_nodes,int number_of_inputs,double output_layer_bias[],double output_weights[],double hidden_layer_bias[],double hidden_weights[]);
void initialization(int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,double hidden_weights[],double hidden_layer_bias[],double output_weights[]);
int forward_pass(int x , int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,int trainingSetOrder[],double training_inputs[],double hidden_weights[],double hidden_layer_bias[],double output_weights[],double output_layer_bias[],double hidden_layer[],double output_layer[]);
void back_propagation(double lr, int i,int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,double training_inputs[],double training_outputs[],double hidden_weights[],double hidden_layer_bias[],double output_weights[],double output_layer_bias[],double hidden_layer[],double output_layer[]);
int cfileexists(const char * filename);
#endif