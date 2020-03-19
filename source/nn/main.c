#include <stdio.h> // used for printf
#include "matrix.h"
#include "sigmoid.h"

/*TODO : Right now the nn only has a "training mode", we should give the user 
the possibility to choose between training the nn and actually using it*/


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
  // TODO : fill in the training inputs and outputs - not working rn, dont't know why
  // Tried : 
  // double training_inputs[numTrainingSets*numInputs] = {0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f};
  // double training_outputs[numTrainingSets*numOutputs] = {0.0f,1.0f,1.0f,0.0f};
  double training_inputs[number_training_sets*number_of_inputs];
  double training_outputs[number_training_sets*number_of_outputs];
  int trainingSetOrder[] = {0,1,2,3};
  const double lr = 0.1f;

  //TODO : See if the file exist ()
  /* Init all the matrices (suppose that the file is empty)*/
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
  printf("Started computing ... \n");
  printf("/ \r");
  for (int n=0; n < 10000; n++)
  {
    shuffle(trainingSetOrder,number_training_sets);
    for (int x=0; x<number_training_sets; x++)
    {
      printf("-- \r");
      /*DONE : Foward pass = actually input some value into
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
      printf("'\' \r");
      for (int j=0; j<number_of_outputs; j++)
      {
        double activation=output_layer_bias[j];
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
          activation+=hidden_layer[k]*output_weights[k*number_of_outputs+j];
        }
        output_layer[j] = sigmoid(activation);
      }
      printf("| \r");
      /* TODO : write down the results contained in ouput_layer[0]
       compared to what the results should have been 
      => Write it into a file to see the evolution*/
      FILE *result_file;

      // WIP : Back propagation = update the weight according to
      // the result that we should have obtained
      double deltaOutput[number_of_inputs];
      for (int j=0; j<number_of_inputs; j++) {
          double errorOutput = (training_outputs[i*number_of_outputs+j]-output_layer[j]);
          deltaOutput[j] = errorOutput*dSigmoid(output_layer[j]);
      }
      printf("/ \r");
      double deltaHidden[number_of_hidden_nodes];
      for (int j=0; j<number_of_hidden_nodes; j++) {
          double errorHidden = 0.0f;
          for(int k=0; k<number_of_inputs; k++) {
              errorHidden+=deltaOutput[k]*output_weights[j*number_of_outputs+k];
          }
          deltaHidden[j] = errorHidden*dSigmoid(hidden_layer[j]);
      }
      printf("-- \r");
      for (int j=0; j<number_of_inputs; j++)
      {
        output_layer_bias[j] += deltaOutput[j]*lr;
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
          output_weights[k+j*number_of_hidden_nodes]+=hidden_layer[k]*deltaOutput[j]*lr;
        }
      }
      printf("'\' \r");
      // TOFIX : Update the weights of hidden layer - Not working (memory leak) when uncomment 
      for(int w = 0;w < number_of_hidden_nodes;w++)
      {
        //hidden_layer_bias[w]+= deltaHidden[w]*lr;
        for(int k=0; k<number_of_inputs; k++) 
        {
          hidden_weights[k*number_of_inputs+w] += lr*training_inputs[i*number_of_inputs+k]; // * deltaHidden[w]
        }
      }
    }
  }
  //TODO : write down the final weight and the biases of the hidden layer and the output layer into Xor-weights.txt
  printf("Done! \n");
  return 0;
}
