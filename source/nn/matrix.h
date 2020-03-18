#ifndef MATRIX_H_
#define MATRIX_H_

void print_matrix(char s[], double m[], size_t rows, size_t cols);
void transpose(double m[][], size_t rows, size_t cols, double r[][]);
int add(double m1[][], double m2[][], size_t rows_m1, size_t cols_m1, size_t rows_m2, size_t cols_m2, double r[][]);
int mul(double m1[][], double m2[][], size_t r1, size_t c1, size_t r2, size_t c2, double r[][]);

#endif