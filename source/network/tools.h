#ifndef TOOLS_H_
#define TOOLS_H_

void progressBar(int step,int nb);
double sigmoid(double x);
double dSigmoid(double x);
float expo(float x);
double init_weight();

int cfileexists(const char * filename);
int fileempty(const char * filename);

void save_network(const char * filename,struct network *network);

void load_network(const char * filename,struct network *network);

void shuffle(int *array, size_t n);

char RetrieveChar(size_t val);

size_t IndexAnswer(struct network *net);

void ExpectedOutput(struct network *network,char c);

size_t ExpectedPos(char c);

char * updatepath(char *filepath,size_t len,char c);

void PrintState(char expected, char obtained);

#endif
