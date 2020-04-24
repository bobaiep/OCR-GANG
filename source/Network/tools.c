#include <stdio.h> 
#include <stdlib.h>
#include "network.h"

float expo(float x) // Self explanitory, just in case cannot use math.h
{ 
    float sum = 1.0f;
    int n = 150; // Arbitrary
    for (int i = n - 1; i > 0; --i )
    {
        sum = 1 + x * sum / i; // Compute by using Taylor's formula
    }
    return sum; 
}

// Activation function and its derivative

double sigmoid(double x)
{
    return 1 / (1 + expo(-x)); 
    // Will be used to adjust the activation of hiddenlayer and outputlayer nodes
}
double dSigmoid(double x)
{ 
    return x * (1 - x); 
    // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
}

// Init all weights and biases between 0.0 and 1.0
double random() 
{ 
    return ((double)rand())/((double)RAND_MAX+1);
}

size_t PosAnswer(struct network *net){
    size_t index_answer = 0;
    for (size_t i = 1; i < net->nbOutput; i++)
    {
        if (net->output[index_answer].activation < net->output[i].activation)
        {
            index_answer = i;
        }
        
    }
    return index_answer;
}

char RetrieveChar(size_t val){
  char c;

  if(val <= 25)
  {
    c = val + 65;
  }
  else if(val > 25 && val <= 51)
  {
    c = (val + 97 - 26);
  }
  else if(val > 51 && val <= 61)
  {
    c = val + 48 - 52;
  }
  else
  {
    switch(val)
    {
      case 62:
        c = ';';
        break;
      case 63:
        c = '\'';
        break;
      case 64:
        c = ':';
        break;
      case 65:
        c = '-';
        break;
      case 66:
        c = '.';
        break;
      case 67:
        c = '!';
        break;
      case 68:
        c = '?';
        break;
      case 69:
        c = '(';
        break;
      case 70:
        c = '\"';
        break;
      case 71:
        c = ')';
        break;
      default:
        exit(1);
        break;
    }
  }
  return c;
}