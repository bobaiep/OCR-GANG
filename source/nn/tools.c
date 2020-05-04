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

void read_file(const char * filename,int number_of_inputs,int number_of_outputs,\
 int number_of_hidden_nodes,double hidden_layer_bias[],double hidden_weights[],\
 double output_layer_bias[],double output_weights[]){
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

void save_weights_bias(int number_of_outputs,int number_of_hidden_nodes,\
int number_of_inputs,double output_layer_bias[],double output_weights[],\
double hidden_layer_bias[],double hidden_weights[] ){
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
