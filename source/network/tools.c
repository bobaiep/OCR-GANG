#include <stdio.h>
#include <stdlib.h>
#include "network.h"

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
#define KGRN  "\x1B[32m"

void progressBar(int step,int nb){
    printf("\e[?25l");
    int percent = (step*100)/nb;
    const int pwidth = 72;
    int pos = (step*pwidth)/nb;
    printf("[");
    for (int i = 0; i < pos; i++)
    {
        printf("%c",'=');
    }
    printf("%*c ",pwidth-pos+1,']');
    printf(" %3d%%\r",percent);
}

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
double init_weight()
{
    return ((double)rand())/((double)RAND_MAX+1);
}

int cfileexists(const char * filename)
{
    /* try to open file to read */
    FILE *file;
    file = fopen(filename, "r");
    if (!file)
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

int fileempty(const char * filename){
    FILE *fptr;
    fptr = fopen( filename, "r" );
    fseek(fptr, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fptr);
    if (len>0){
        return 0;
    }
    else
    {
        return 1;
    }

}

void save_network(const char * filename,struct network *network)
{
    FILE *output = fopen(filename,"w");
    for (int k = 0; k < network -> number_of_inputs; k++)
    {
      for (int o = 0; o < network -> number_of_hidden_nodes; o++)
      {
        fprintf(output,"%f %f %f %f %f\n",network->hidden_layer[o],network-> delta_hidden[o],\
        network->hidden_layer_bias[o],network -> hidden_weights[k*network -> number_of_hidden_nodes+o],\
        network->delta_hidden_weights[k*network -> number_of_hidden_nodes+o]);
      }
    }
    for (int i = 0; i < network -> number_of_hidden_nodes; i++)
    {
      for (int a = 0; a < network -> number_of_outputs; a++)
      {
        fprintf(output,"%f %f %f %f %f\n",network->output_layer[a],network-> delta_output[a],\
        network->output_layer_bias[a],network -> output_weights[i*network -> number_of_outputs+a],\
        network->delta_output_weights[i*network -> number_of_outputs+a]);
      }
    }
    fclose(output);
}

void load_network(const char * filename,struct network *network){
    FILE *input = fopen(filename,"r");
    for (int k = 0; k < network -> number_of_inputs; k++)
    {
      for (int o = 0; o < network -> number_of_hidden_nodes; o++)
      {
        fscanf(input,"%lf %lf %lf %lf %lf\n",&network->hidden_layer[o],&network-> delta_hidden[o],\
        &network->hidden_layer_bias[o],&network -> hidden_weights[k*network -> number_of_hidden_nodes+o],\
        &network->delta_hidden_weights[k*network -> number_of_hidden_nodes+o]);
      }
    }
    for (int i = 0; i < network -> number_of_hidden_nodes; i++)
    {
      for (int a = 0; a < network -> number_of_outputs; a++)
      {
        fscanf(input,"%lf %lf %lf %lf %lf\n",&network->output_layer[a],&network-> delta_output[a],\
        &network->output_layer_bias[a],&network -> output_weights[i*network -> number_of_outputs+a],\
        &network->delta_output_weights[i*network -> number_of_outputs+a]);
      }
    }
    fclose(input);
}

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

size_t IndexAnswer(struct network *net){
    size_t index = 0;
    for (size_t i = 1; i < (size_t)net->number_of_outputs; i++) {
        if (net->output_layer[i]>net->output_layer[index]){
            index = i;
        }
    }
    return index;
}
char RetrieveChar(size_t val)
{
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

size_t ExpectedPos(char c){
  size_t index= (size_t)c;
  if ( c >= 'A' && c <= 'Z') {
    index -= 65;
  }
  if (c >= 'a' && c <= 'z') {
    index -= 71;
  }
  return index;
}

void ExpectedOutput(struct network *network,char c) {
  size_t index  = ExpectedPos(c);
  for (size_t i = 0; i < (size_t)network->number_of_outputs; i++) {
    if ( i == index) {
      network->goal[i] = 1;
    }
    else{
      network->goal[i] = 0;
    }
  }

}

/*void PrintState(struct network *net, char expected, char obtained)
{
  //Squared error function
  SquaredError(net);
  int output = RetrievePos(net);

  //Retrive the chars : wanted & found
  char goalChar = RetrieveChar(PosGoal(net -> Goal));
  char recognizedChar = RetrieveChar(output);

  if(net -> ErrorRate > net -> MaxErrorRate)
    net -> MaxErrorRate = net -> ErrorRate;

  //Print the progress
  if(output == PosGoal(net -> Goal))
    printf("Position Found = %d Expected %d %sOK \n",
                    output, PosGoal(net -> Goal),KGRN);
  else
    printf("Position Found = %d Expected %d %sKO \n",
                    output, PosGoal(net -> Goal),KRED);

  printf("%s",KWHT);

  printf("Char entered: %c | Char recoginized: %c | ErrorRate: %f\n",
                                                    goalChar,
                                                    recognizedChar,
                                                    net -> ErrorRate);
}*/
