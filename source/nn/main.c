#include <stdio.h> // used for printf
#include "matrix.h"
#include "sigmoid.h"
#include <err.h> // used for error 

int main(){
  static const int number_of_inputs = 2;
  static const int number_of_hidden_nodes = 2;
  static const int number_of_outputs = 1;
  double hidden_layer[number_of_hidden_nodes];
  double output_layer[number_of_outputs];
  double hidden_layer_bias[number_of_hidden_nodes];
  double output_layer_bias[number_of_outputs];
  double hidden_weights[number_of_inputs][number_of_hidden_nodes];
  double output_weights[number_of_hidden_nodes][number_of_outputs];

  /* Init all the matrices needed */
  for (int i=0; i<number_of_inputs; i++)
  {
    for (int j=0; j<number_of_hidden_nodes; j++)
    {
      hidden_weights[i][j] = init_weight();
    }
  }
  for (int i = 0; i < number_of_hidden_nodes; i++)
  {
	  hidden_layer_bias[i] = init_weight();
	  for (int j = 0; j < number_of_outputs; j++)
	  {
		  output_weights[i][j] = init_weight();
	  }
  }
  for (int i = 0; i < number_of_hidden_nodes; i++)
  {
    output_layer_bias[i] = init_weight();
  }

  FILE* in_file = fopen("Xor-data.txt",'r'); // open file in read mode
  if (! in_file ) // equivalent to saying if ( in_file == NULL ) 
  {  
    errx(1,"Cannot read Xor-data.txt ! Please verify this file.\n");
    }
  size_t i = 0;
  char line[13]; // To store the result of the line in Xor-data.txt
  while (fgets( line, 13, stdin ) != NULL )
  {
    while (line[i]!=' ') // Construction of the number
    {
      i++;
    }
    
  } 
  return 0;
}
