// Below we define the dimensions of the neural network 
// and we allocate the arrays for the layers, biases and weights: 

#include <err.h> // used for error - OK 
#include <stddef.h> // used for size_t type, idk why but compiler said it was needed so 
#include <stdio.h> // used for printf - OK
#define RAND_MAX 0x7fffffff

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s  =",s);
    printf("\n");
    for (size_t i = 0; i < rows; i++) 
    {
        for (size_t j = 0; j < cols; j ++)
        {
            printf("%4g ",m[i * cols + j]);
        }
        printf("\n");
    }
    
}
void transpose(double m[], size_t rows, size_t cols, double r[])
{
    for (size_t i = 0; i < rows; i++) 
    {
        for (size_t j = 0; j < cols; j ++)
        {
            r[j * rows + i] = m[i * cols + j];
        }
    }
}

void add(double m1[], double m2[], size_t rows_m1, size_t cols_m1, size_t rows_m2, size_t cols_m2, double r[])
{
    if(rows_m1 == rows_m2 && cols_m1 == cols_m2)
    {
        for (size_t i = 0; i < rows_m1; i++)
        {
            for (size_t j = 0; j < cols_m1; j ++)
            {
               r[i * cols_m1 + j] = m1[i * cols_m1 + j] + m2[i * cols_m1 + j] ;
            }
        }
    }
    else {
        printf("Matrices are not the same size ! \n");
    }
}

void mul(double m1[], double m2[], size_t r1, size_t c1, size_t r2, size_t c2, double r[])
{
    if (r1 == c2 && c1 == r2)
    {
        for (size_t i = 0; i < r1; i++)
        {
            for (size_t j = 0; j < c2; j++)
            {
                r[i * c2 + j] = 0;
                for (size_t k = 0; k < c1; k++)
                {
                    r[i * c2 + j] += m1[i * c1 + k] * m2[k * c2 + j];
                }
            }
        }
    }
    else {
        printf("Matrices are not the same size ! \n");
    }
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