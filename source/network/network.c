// File for Sigmoid functions in particular and other functions needed for
// operating the neural network Used for tweaking the weigth of each node in the
// neural network
#include "network.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "tools.h"

struct network *InitializeNetwork(double i, double h, double o, char *filepath)
{
    struct network *network = malloc(sizeof(struct network));
    if (network == NULL)
    {
        errx(1, "Not enough memory!");
    }
    network->number_of_inputs = i;
    network->number_of_hidden_nodes = h;
    network->number_of_outputs = o;
    network->input_layer = calloc(network->number_of_inputs, sizeof(double));

    network->hidden_layer =
        calloc(network->number_of_hidden_nodes, sizeof(double));
    network->delta_hidden =
        calloc(network->number_of_hidden_nodes, sizeof(double));
    network->hidden_layer_bias =
        calloc(network->number_of_hidden_nodes, sizeof(double));
    network->hidden_weights =
        calloc(network->number_of_inputs * network->number_of_hidden_nodes,
               sizeof(double));
    network->delta_hidden_weights =
        calloc(network->number_of_inputs * network->number_of_hidden_nodes,
               sizeof(double));

    network->output_layer = calloc(network->number_of_outputs, sizeof(double));
    network->delta_output = calloc(network->number_of_outputs, sizeof(double));
    network->output_layer_bias =
        calloc(network->number_of_outputs, sizeof(double));
    network->output_weights =
        calloc(network->number_of_hidden_nodes * network->number_of_outputs,
               sizeof(double));
    network->delta_output_weights =
        calloc(network->number_of_hidden_nodes * network->number_of_outputs,
               sizeof(double));

    network->goal = calloc(network->number_of_outputs, sizeof(double));
    network->eta = 0.5f;
    network->alpha = 0.9f;

    if (!fileempty(filepath))
    {
        load_network(filepath, network);
    }
    else
    {
        initialization(network);
    }
    return network;
}

void initialization(struct network *net)
{
    for (int i = 0; i < net->number_of_inputs; i++)
    {
        for (int j = 0; j < net->number_of_hidden_nodes; j++)
        {
            net->hidden_layer_bias[j] = init_weight();
            net->hidden_weights[i * net->number_of_hidden_nodes + j] =
                init_weight();
        }
    }
    for (int k = 0; k < net->number_of_hidden_nodes; k++)
    {
        for (int l = 0; l < net->number_of_outputs; l++)
        {
            net->output_layer_bias[l] = init_weight();
            net->output_weights[k * net->number_of_outputs + l] = init_weight();
        }
    }
}

void forward_pass(struct network *net)
{
    /*DONE : Foward pass = actually input some value into
    the neural network and see what we obtain out of it*/

    for (int j = 0; j < net->number_of_hidden_nodes; j++)
    {
        double activation = net->hidden_layer_bias[j];
        for (int k = 0; k < net->number_of_inputs; k++)
        {
            activation += net->input_layer[k]
                * net->hidden_weights[k * net->number_of_hidden_nodes + j];
        }
        net->hidden_layer[j] = sigmoid(activation);
    }
    for (int j = 0; j < net->number_of_outputs; j++)
    {
        double activation = net->output_layer_bias[j];
        for (int k = 0; k < net->number_of_hidden_nodes; k++)
        {
            activation += net->hidden_layer[k]
                * net->output_weights[k * net->number_of_outputs + j];
        }
        net->output_layer[j] = sigmoid(activation);
    }
}

void back_propagation(struct network *net)
{
    /*DONE : Back propagation = update the weight according to
    what we should have obtained out of the neural network*/

    for (int o = 0; o < net->number_of_outputs; o++)
    {
        net->delta_output[o] = (net->goal[o] - net->output_layer[o])
            * dSigmoid(net->output_layer[o]);
    }
    double sum;
    for (int h = 0; h < net->number_of_hidden_nodes; h++)
    {
        sum = 0.0;
        for (int o = 0; o < net->number_of_outputs; o++)
        {
            sum += net->output_weights[h * net->number_of_outputs + o]
                * net->delta_output[o];
        }
        net->delta_hidden[h] = sum * dSigmoid(net->hidden_layer[h]);
    }
}

void updateweightsetbiases(struct network *net)
{
    // Weights and biases between Input and Hidden layers
    for (int i = 0; i < net->number_of_inputs; i++)
    {
        for (int j = 0; j < net->number_of_hidden_nodes; j++)
        {
            net->hidden_weights[i * net->number_of_hidden_nodes + j] +=
                net->eta * net->input_layer[i] * net->delta_hidden[j];
            net->hidden_layer_bias[j] += net->eta * net->delta_hidden[j];
        }
    }

    // Weights between Hidden and Ouput layers
    for (int o = 0; o < net->number_of_outputs; o++)
    {
        for (int h = 0; h < net->number_of_hidden_nodes; h++)
        {
            net->output_weights[h * net->number_of_outputs + o] +=
                net->eta * net->delta_output[o] * net->hidden_layer[h]
                + net->alpha
                    * net->delta_output_weights[h * net->number_of_outputs + o];

            net->delta_output_weights[h * net->number_of_outputs + o] =
                net->eta * net->delta_output[o] * net->hidden_layer[h];
        }
        net->output_layer_bias[o] += net->eta * net->delta_output[o];
    }
}

int InputImage(struct network *net, size_t index, int ***chars_matrix)
{
    int is_espace = 1;
    for (size_t i = 0; i < 784; i++)
    {
        net->input_layer[i] = (*chars_matrix)[index][i];
        if (net->input_layer[i] == 1)
        {
            is_espace = 0;
        }
    }
    return is_espace;
}
