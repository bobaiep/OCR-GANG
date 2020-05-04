void progressBar(int step,int nb);
double sigmoid(double x);
double dSigmoid(double x);
float expo(float x);
double init_weight();

int cfileexists(const char * filename);
int fileempty(const char * filename);

void save_weights_bias(int number_of_outputs,int number_of_hidden_nodes,int number_of_inputs,\
double output_layer_bias[],double output_weights[],double hidden_layer_bias[],double hidden_weights[]);

void read_file(const char * filename,int number_of_inputs,int number_of_outputs, \
int number_of_hidden_nodes,double hidden_layer_bias[],double hidden_weights[],double output_layer_bias[],double output_weights[]);

void shuffle(int *array, size_t n);
