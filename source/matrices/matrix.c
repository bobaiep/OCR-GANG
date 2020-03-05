// Below we define the dimensions of the neural network 
// and we allocate the arrays for the layers, biases and weights: 

static const int number_of_inputs = 2;
static const int number_of_hidden_nodes = 2;
static const int number_of_outputs = 1;
double hidden_layer[numHiddenNodes];
double output_layer[numOutputs];
double hidden_layer_bias[numHiddenNodes];
double output_layer_bias[numOutputs];
double hidden_weights[numInputs][numHiddenNodes];
double output_weights[numHiddenNodes][numOutputs];

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s  =",s);
    printf("\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j ++){
            printf("%4g",m[i * cols + j]);
            printf(" ");
        };
        printf("\n");
    };
    
}

void transpose(double m[], size_t rows, size_t cols, double r[])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j ++){
            r[j * rows + i] = m[i * cols + j];
        };
    };
}

void add(double m1[], double m2[], size_t rows, size_t cols, double r[])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j ++){
            r[i * cols + j] = m1[i * cols + j] + m2[i * cols + j] ;
        };
    };
}

void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[])
{
    for (size_t i = 0; i < r1; i++) {
        for (size_t j = 0; j < c2; j++) {
            r[i * c2 + j] = 0;
            for (size_t k = 0; k < c1; k++) {
                r[i * c2 + j] += m1[i * c1 + k] * m2[k * c2 + j];
            }
            
        }
    }
}