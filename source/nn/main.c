#include <stdio.h> // used for printf
#include "matrix.h"
#include "sigmoid.h"

int main(){
  /* Declare all we are gonna need */
  static const int number_of_inputs = 2;
  static const int number_of_hidden_nodes = 2;
  static const int number_of_outputs = 1;
  double hidden_layer[number_of_hidden_nodes];
  double output_layer[number_of_outputs];
  double hidden_layer_bias[number_of_hidden_nodes];
  double output_layer_bias[number_of_outputs];
  double hidden_weights[number_of_inputs*number_of_hidden_nodes];
  double output_weights[number_of_hidden_nodes*number_of_outputs];
  static const int number_training_sets = 4;
  double training_inputs[number_training_sets*number_of_inputs];
  double training_outputs[number_training_sets*number_of_outputs];
  int trainingSetOrder[] = {0,1,2,3};

  /* Init all the matrices*/
  for (int i=0; i<number_of_inputs; i++)
  {
    for (int j=0; j<number_of_hidden_nodes; j++)
    {
      hidden_weights[i*number_of_hidden_nodes+j] = init_weight();
    }
  }
  for (int i = 0; i < number_of_hidden_nodes; i++)
  {
    hidden_layer_bias[i] = init_weight();
    for (int j = 0; j < number_of_outputs; j++)
    {
      output_weights[i*number_of_outputs+j] = init_weight();
    }
  }
  for (int i = 0; i < number_of_hidden_nodes; i++)
  {
    output_layer_bias[i] = init_weight();
  }
  printf("Finished all initialization !\n");
  
  for (int n=0; n < 10000; n++)
  {
    shuffle(trainingSetOrder,number_training_sets);
    for (int x=0; x<number_training_sets; x++)
    {
      /*WIP : Foward pass = actually input some value into 
      the neural network and see what we obtain out of it*/
      int i = trainingSetOrder[x]; // Select a random tuple
      for (int j=0; j<number_of_hidden_nodes; j++)
      {
        double activation=hidden_layer_bias[j];
        for (int k=0; k<number_of_inputs; k++) 
        {
          activation+=training_inputs[i*number_of_inputs+k]*hidden_weights[k*number_of_hidden_nodes+j];
        }
        hidden_layer[j] = sigmoid(activation);
      }

      for (int j=0; j<number_of_outputs; j++)
      {
        double activation=output_layer_bias[j];
        for (int k=0; k<number_of_hidden_nodes; k++) 
        {
          activation+=hidden_layer[k]*output_weights[k*number_of_outputs+j];
        }
        output_layer[j] = sigmoid(activation);
      }
      /* The output is at output_layer[0] => Write it into a file to see the evolution*/


      // TODO : Back propagation = update the weight according to
      // the result that we should have obtained
    }
  }
  return 0;
}
