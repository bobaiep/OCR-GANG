// File for Sigmoid functions in particular and other functions needed for operating the neural network
// Used for tweaking the weigth of each node in the neural network
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
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
  network->input_layer = calloc(network->number_of_inputs,sizeof(double));

  network->hidden_layer = calloc(network->number_of_hidden_nodes,sizeof(double));
  network-> delta_hidden = calloc(network ->number_of_hidden_nodes,sizeof(double));
  network->hidden_layer_bias = calloc(network->number_of_hidden_nodes,sizeof(double));
  network->hidden_weights = calloc(network->number_of_inputs * network->number_of_hidden_nodes,sizeof(double));
  network->delta_hidden_weights = calloc(network->number_of_inputs * network->number_of_hidden_nodes,sizeof(double));

  network->output_layer = calloc(network->number_of_outputs,sizeof(double));
  network-> delta_output = calloc(network -> number_of_outputs,sizeof(double));
  network->output_layer_bias = calloc(network->number_of_outputs,sizeof(double));
  network->output_weights = calloc(network->number_of_hidden_nodes * network->number_of_outputs,sizeof(double));
  network->delta_output_weights = calloc(network->number_of_hidden_nodes * network->number_of_outputs,sizeof(double));

  network->goal = calloc(network->number_of_outputs,sizeof(double));
  network->eta = 0.5f;
  network -> alpha = 0.9f;

  if(!fileempty("source/Xor/xorwb.txt"))
  {
      load_network("source/Xor/xorwb.txt",network);
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

void forward_pass(struct network *net){
    /*DONE : Foward pass = actually input some value into
    the neural network and see what we obtain out of it*/

    for (int j=0; j< net -> number_of_hidden_nodes; j++)
    {
        double activation= net ->hidden_layer_bias[j];
        for (int k=0; k< net ->number_of_inputs; k++)
        {
          activation+=net->input_layer[k]*net ->hidden_weights[k*net ->number_of_hidden_nodes+j];
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
}

void back_propagation(struct network *net){
    /*DONE : Back propagation = update the weight according to
    what we should have obtained out of the neural network*/
    double output, derivate, goal, error;
    for (int o=0; o<net ->number_of_outputs; o++)
    {
        output = net -> output_layer[o];
        derivate = dSigmoid(output);
        goal = net -> goal[o];
        error = (goal - output) * derivate;

        net->delta_output[o] = error;
    }
    double sum, weight, delta;
    for (int h = 0; h < net -> number_of_hidden_nodes; h++)
    {
        sum = 0.0;
        for (int o = 0; o < net -> number_of_outputs; o++)
        {
            weight = net -> output_weights[h*net->number_of_outputs + o];
            delta = net->delta_output[o];

            sum += weight * delta;
        }
        output = net -> hidden_layer[h];
        derivate = dSigmoid(output);
        net->delta_hidden[h] = sum * derivate;

    }
}

void updateweights(struct network *net){

    double eta, alpha, error, output, dWeight;
    eta = net -> eta;
    alpha = net -> alpha;

    //Weights between Input and Hidden layers
    for (int h = 0; h < net -> number_of_hidden_nodes; h++)
    {
        for(int i = 0; i < net -> number_of_inputs; i++)
        {
            output = net -> input_layer[i];
            error = net -> delta_hidden[h];
            dWeight = net -> delta_hidden_weights[i*net -> number_of_hidden_nodes+h];

            net -> hidden_weights[i*net -> number_of_hidden_nodes+h] += eta * error * output +
                                        alpha * dWeight;
            net -> delta_hidden_weights[i*net -> number_of_hidden_nodes+h] = eta * error * output;
        }
    }

    //Weights between Hidden and Ouput layers
    for (int o = 0; o < net -> number_of_outputs; o++)
    {
        for (int h = 0; h < net -> number_of_hidden_nodes; h++)
        {
            output = net -> hidden_layer[h];
            error = net -> delta_output[o];
            dWeight = net -> delta_output_weights[h*net -> number_of_outputs+o];

            net -> output_weights[h*net -> number_of_outputs+o] += eta * error * output +
                                        alpha * dWeight;
            net -> delta_output_weights[h*net -> number_of_outputs+o] = eta * error * output;
        }
    }
}

void UpdateBiases(struct network *net)
{
  double eta, delta, dBias;
  eta = net -> eta;

  //Update BiasH
  for (int h = 0; h < net -> number_of_hidden_nodes; h++)
  {
    delta = net -> delta_hidden[h];
    dBias = eta * delta;

    net -> hidden_layer_bias[h] += dBias;
  }

  //Update BiasO
  for (int o = 0; o < net -> number_of_outputs; o++)
  {
    delta = net -> delta_output[o];
    dBias = eta * delta;

    net -> output_layer_bias[o] += dBias;
  }

}

void InputImage(struct network *net,size_t index,int ***chars_matrix){
    for (size_t i = 0; i < 900; i++) {
        net->input_layer[i] = (*chars_matrix)[index][i];
    }
}
