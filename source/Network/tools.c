#include <stdio.h> 
#include <stdlib.h>
#include "network.h"
#include <err.h>

float exponential(float x) // Self explanitory, just in case cannot use math.h
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

double fsigmoid(double x)
{
    return 1 / (1 + exponential(-x)); 
    // Will be used to adjust the activation of hiddenlayer and outputlayer nodes
}
double derivatesigmoid(double x)
{ 
    return x * (1 - x); 
    // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
}

// Init all weights and biases between 0.0 and 1.0
double initneuron()
{ 
    return ((double)rand())/((double)RAND_MAX+1);
}

void savenetwork(struct network *net){
  FILE *output;
  FILE *hidden;
  printf("pointer initialized\n");
  output = fopen("source/Network/outputwb.txt","w");
  hidden = fopen("source/Network/hiddenwb.txt","w");
  for (size_t o = 0; o < net -> nbOutput; o++)
  {
    fprintf(output,"%f %f %f %f %f\n",net->output[o].activation,net->output[o].weight,net->output[o].biais,net->output[o].delta,net->output[o].deltaweight);
  }
  for (size_t o = 0; o < net -> nbHidden; o++)
  {
    fprintf(hidden,"%f %f %f %f %f\n",net->hidden[o].activation,net->hidden[o].weight,net->hidden[o].biais,net->hidden[o].delta,net->hidden[o].deltaweight);
  }
  fclose(output);
  fclose(hidden);
}

void loadnetwork(struct network *net){
  FILE *hidden = fopen("source/Network/hiddenwb.txt","r");
  FILE *output = fopen("source/Network/outputwb.txt","r");
  if (hidden == NULL || output == NULL)
  {
    errx(1,"It seems that there was a problem with the loading of the neural network, oups..\n");
  }
  for (size_t o = 0; o < net->nbHidden; o++)
  {
    fscanf(hidden,"%lf %lf %lf %lf %lf",&net->hidden[o].activation,&net->hidden[o].weight,&net->hidden[o].biais,&net->hidden[o].delta,&net->hidden[o].deltaweight);
  }
  for (size_t o = 0; o < net->nbOutput; o++)
  {
    fscanf(output,"%lf %lf %lf %lf %lf",&net->output[o].activation,&net->output[o].weight,&net->output[o].biais,&net->output[o].delta,&net->output[o].deltaweight);
  }
  fclose(hidden);
  fclose(output);
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