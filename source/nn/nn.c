// File for Sigmoid functions in particular and other functions needed for operating the neural network
// Used for tweaking the weigth of each node in the neural network
#include <stdio.h>
#include <stdlib.h>
#include "nn.h"
#include <err.h>
#include "tools.h"

struct network* InitializeNetwork(double i, double h, double o){
  struct network *network = malloc(sizeof(struct network));
  if (network == NULL){
    errx(1, "Not enough memory!");
  }
  network-> number_of_inputs = i;
  network-> number_of_hidden_nodes = h;
  network-> number_of_outputs = o;
  network->hidden_layer = calloc(network->number_of_hidden_nodes,sizeof(double));
  network->hidden_layer_bias = calloc(network->number_of_hidden_nodes,sizeof(double));
  network->hidden_weights = calloc(network->number_of_inputs * network->number_of_hidden_nodes,sizeof(double));
  network->output_layer = calloc(network->number_of_outputs,sizeof(double));
  network->output_layer_bias = calloc(network->number_of_outputs,sizeof(double));
  network->output_weights = calloc(network->number_of_hidden_nodes * network->number_of_outputs,sizeof(double));
  network->lr = 0.2f;

  if(!fileempty("source/Xor/Xor-weights.txt"))
  {
      read_file("source/Xor/Xor-weights.txt",network->number_of_inputs,network->number_of_outputs, \
      network->number_of_hidden_nodes,network->hidden_layer_bias,network->hidden_weights,network->output_layer_bias,network->output_weights);
  }
  else{
      initialization(network);
  }

  return network;
}

void initialization(struct network *net){
    for (int i=0; i<net ->number_of_inputs; i++)
    {
        for (int j=0; j<net ->number_of_hidden_nodes; j++)
        {
            net ->hidden_weights[i*net ->number_of_hidden_nodes+j] = init_weight();
        }
    }
    for (int i = 0; i < net ->number_of_hidden_nodes; i++)
    {
        net ->hidden_layer_bias[i] = init_weight();
        for (int j = 0; j < net ->number_of_outputs; j++)
        {
            net ->output_weights[i*net ->number_of_outputs+j] = init_weight();
        }
    }
}

int forward_pass(int x ,struct network *net,int trainingSetOrder[],\
  double training_inputs[]){
    /*DONE : Foward pass = actually input some value into
    the neural network and see what we obtain out of it*/
    int i = trainingSetOrder[x]; // Select a random tuple
    for (int j=0; j< net -> number_of_hidden_nodes; j++)
    {
        double activation= net ->hidden_layer_bias[j];
        for (int k=0; k< net ->number_of_inputs; k++)
        {
          activation+=training_inputs[i* net ->number_of_inputs+k]*net ->hidden_weights[k*net ->number_of_hidden_nodes+j];
        }
        net ->hidden_layer[j] = sigmoid(activation);
    }
    for (int j=0; j<net ->number_of_outputs; j++)
    {
        double activation= net ->output_layer_bias[j];
        for (int k=0; k< net ->number_of_hidden_nodes; k++)
        {
          activation+= net ->hidden_layer[k]* net ->output_weights[k* net ->number_of_outputs+j];
        }
        net ->output_layer[j] = sigmoid(activation);
    }
    return i;
}

void back_propagation(struct network *net, int i,double training_inputs[],double training_outputs[]){
    /*DONE : Back propagation = update the weight according to
    what we should have obtained out of the neural network*/
    double deltaOutput[net ->number_of_inputs];
    for (int j=0; j<net ->number_of_outputs; j++)
    {
        double errorOutput = (training_outputs[i*net ->number_of_outputs+j]-net ->output_layer[j]);
        deltaOutput[j] = errorOutput*dSigmoid(net ->output_layer[j]);
    }
    double deltaHidden[net ->number_of_hidden_nodes];
    for (int j=0; j<net ->number_of_hidden_nodes; j++)
    {
        double errorHidden = 0.0f;
        for(int k=0; k<net ->number_of_inputs; k++)
        {
            errorHidden+=deltaOutput[k]*net ->output_weights[j*net ->number_of_hidden_nodes+k];
        }
        deltaHidden[j] = errorHidden*dSigmoid(net ->hidden_layer[j]);
    }
    for (int j=0; j< net ->number_of_outputs; j++)
    {
        net ->output_layer_bias[j] += deltaOutput[j]*net->lr;
        for (int k=0; k< net ->number_of_hidden_nodes; k++)
        {
            net ->output_weights[k+j*net ->number_of_hidden_nodes]+= net ->hidden_layer[k]*deltaOutput[j]*net ->lr;
        }
    }
    for(int w = 0;w < net ->number_of_hidden_nodes;w++)
    {
        net ->hidden_layer_bias[w] += deltaHidden[w]*net ->lr;
        for(int k=0; k<net ->number_of_inputs; k++)
        {
            net ->hidden_weights[k*net ->number_of_inputs+w] += net ->lr*training_inputs[i*net ->number_of_inputs+k]* deltaHidden[w];
        }
    }
}
