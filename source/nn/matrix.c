// Below we define the dimensions of the neural network 
// and we allocate the arrays for the layers, biases and weights: 

#include <err.h> // used for error 

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s  =",s);
    printf("\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j ++){
            printf("%4g ",m[i][j]);
        };
        printf("\n");
    };
    
}

void transpose(double m[][], size_t rows, size_t cols, double r[][])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j ++){
            r[i][j] = m[j][i];
        }
    }
}

int add(double m1[][], double m2[][], size_t rows_m1, size_t cols_m1, size_t rows_m2, size_t cols_m2, double r[][])
{
    if(rows_m1 == rows_m2 && cols_m1 == cols_m2)
    {
        for (size_t i = 0; i < rows_m1; i++)
        {
            for (size_t j = 0; j < cols_m1; j ++)
            {
                r[i][j] = m1[i][j] + m2[i][j] ;
            }
        }
        return 0;
    }
    else {
        errx(1,"Matrices are not the same size ! \n");
    }
}

int mul(double m1[][], double m2[][], size_t r1, size_t c1, size_t r2, size_t c2, double r[][])
{
    if (r1 == c2)
    {
        for (size_t i = 0; i < r1; i++)
        {
            for (size_t i = 0; i < c2; i++)
            {
                r[i][j] = 0; 
                for (k = 0; k < c1; k++){
                    r[i][j] += m1[i][k]*m2[k][j];
                }
            }
        }
        return 0;
    }
    else {
        errx(1,"Matrices are not the same size ! \n");
    }
}