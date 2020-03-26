// File for Sigmoid functions in particular and other functions needed for operating the neural network
// Used for tweaking the weigth of each node in the neural network
#include <stdio.h> 
#include <stdlib.h> 

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
    return 1 / (1 + expo(-x)); // Will be used to adjust the activation of hiddenlayer and outputlayer nodes
}
double dSigmoid(double x)
{ 
    return x * (1 - x); // Will be used to compute the weigth of hiddenlayer and outputlayer nodes
}

// Init all weights and biases between 0.0 and 1.0
double init_weight() 
{ 
    return ((double)rand())/((double)RAND_MAX); // RAND_MAX is a constant defined above
}

int cfileexists(const char * filename) //NOT WORKING RN
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

void read_file(const char * filename,int number_of_inputs,int number_of_outputs, int number_of_hidden_nodes,double hidden_layer_bias[],double hidden_weights[],double output_layer_bias[],double output_weights[]){
    FILE *fptr;
    fptr = fopen( filename, "r" );
    for (int i = 0; i < number_of_hidden_nodes; i++)
    {
        fscanf(fptr," %lf",&hidden_layer_bias[i]);
    }
    for(int w = 0;w < number_of_hidden_nodes;w++)
    {
        for(int k=0; k<number_of_inputs; k++) 
        {
            fscanf(fptr," %lf",&hidden_weights[k*number_of_inputs+w]);
        }
    }
    for (int j=0; j<number_of_outputs; j++)
    {
        fscanf(fptr," %lf",&output_layer_bias[j]);
    }
    for (int j=0; j<number_of_outputs; j++)
    {
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
            fprintf(fptr," %f",output_weights[k+j*number_of_hidden_nodes]);
        }
    }
    fclose(fptr);
}

void save_weights_bias(int number_of_outputs,int number_of_hidden_nodes,int number_of_inputs,double output_layer_bias[],double output_weights[],double hidden_layer_bias[],double hidden_weights[] ){
    FILE *weights_and_biases ;
    printf("Saving weights and biases...\n");
    weights_and_biases = fopen("source/Xor/Xor-weights.txt","w");
    for(int w = 0;w < number_of_hidden_nodes;w++)
    {
        fprintf(weights_and_biases, " %f",hidden_layer_bias[w]);
    }
    fprintf(weights_and_biases, "\n");
    for(int w = 0;w < number_of_hidden_nodes;w++)
    {
        for(int k=0; k<number_of_inputs; k++) 
        {
            fprintf(weights_and_biases, " %f",hidden_weights[k*number_of_inputs+w]);
        }
    }
    fprintf(weights_and_biases, "\n");
    for (int j=0; j<number_of_outputs; j++)
    {
        fprintf(weights_and_biases, " %f", output_layer_bias[j]);
    }
    fprintf(weights_and_biases, "\n");
    for (int j=0; j<number_of_outputs; j++)
    {
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
            fprintf(weights_and_biases, " %f",output_weights[k+j*number_of_hidden_nodes]);
        }
    }
    fprintf(weights_and_biases, "\n");
    printf("Done! \n");
    fclose(weights_and_biases);
}

void initialization(int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,double hidden_weights[],double hidden_layer_bias[],double output_weights[]){
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
}

int forward_pass(int x , int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,int trainingSetOrder[],double training_inputs[],double hidden_weights[],double hidden_layer_bias[],double output_weights[],double output_layer_bias[],double hidden_layer[],double output_layer[]){
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
    for (int j=0; j<number_of_outputs; j++)
    {
        double activation=output_layer_bias[j];
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
        activation+=hidden_layer[k]*output_weights[k*number_of_outputs+j];
        }
        output_layer[j] = sigmoid(activation);
    }
    return i;
}

void back_propagation(double lr, int i,int number_of_inputs,int number_of_hidden_nodes,int number_of_outputs,double training_inputs[],double training_outputs[],double hidden_weights[],double hidden_layer_bias[],double output_weights[],double output_layer_bias[],double hidden_layer[],double output_layer[]){
    /*DONE : Back propagation = update the weight according to 
    what we should have obtained out of the neural network*/
    double deltaOutput[number_of_inputs];
    for (int j=0; j<number_of_outputs; j++) {
        double errorOutput = (training_outputs[i*number_of_outputs+j]-output_layer[j]);
        deltaOutput[j] = errorOutput*dSigmoid(output_layer[j]);
    }
    double deltaHidden[number_of_hidden_nodes];
    for (int j=0; j<number_of_hidden_nodes; j++) {
        double errorHidden = 0.0f;
        for(int k=0; k<number_of_inputs; k++) {
            errorHidden+=deltaOutput[k]*output_weights[j*number_of_outputs+k];
        }
        deltaHidden[j] = errorHidden*dSigmoid(hidden_layer[j]);
    }
    for (int j=0; j<number_of_outputs; j++)
    {
        output_layer_bias[j] += deltaOutput[j]*lr;
        for (int k=0; k<number_of_hidden_nodes; k++)
        {
            output_weights[k+j*number_of_hidden_nodes]+=hidden_layer[k]*deltaOutput[j]*lr;
        }
    }
    for(int w = 0;w < number_of_hidden_nodes;w++)
    {
        hidden_layer_bias[w] += deltaHidden[w]*lr;
        for(int k=0; k<number_of_inputs; k++) 
        {
            hidden_weights[k*number_of_inputs+w] += lr*training_inputs[i*number_of_inputs+k]* deltaHidden[w];
        }
    }
}