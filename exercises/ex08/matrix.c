/* Example code for Exercises in C.

Copyright 2016 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int num_rows, num_cols;
    double **rows;
} Matrix;

/* Make a matrix.
*/
Matrix *make_matrix(int num_rows, int num_cols) {
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->num_rows = num_rows;
    matrix->num_cols = num_cols;
    matrix->rows = malloc(num_rows * sizeof(double*));
    for (int i=0; i<num_rows; i++) {
        matrix->rows[i] = calloc(num_cols, sizeof(double));
    }
    return matrix;
}

/* Free a matrix.
*/
void free_matrix(Matrix *matrix) {
    // Iterate through Matrix
    for (int i=0; i<matrix->num_rows; i++){
      // Free each row
      double* row = matrix->rows[i];
      free(row);
    }
    free(matrix->rows);
    free(matrix);
}

/* Print a row of a matrix.
*/
void print_matrix_row(double *row, int num_cols) {
    for (int j=0; j<num_cols; j++) {
        printf("%f ", row[j]);
    }
}

/* Print a matrix.
*/
void print_matrix(Matrix *matrix) {
    for (int i=0; i<matrix->num_rows; i++) {
        print_matrix_row(matrix->rows[i], matrix->num_cols);
        printf("\n");
    }
}

/* Perform row reduction.

Subtract a multiple of row j from row i so that the first element
of row i is 0.
*/
void reduce_matrix_rows(Matrix *matrix, int i, int j) {
    double* row1 = matrix->rows[i];
    double* row2 = matrix->rows[j];

    double multiple = row1[0]/row2[0];

    for (int i = 0; i< matrix->num_cols; i++){
      row1[i] = row1[i] - (multiple*row2[i]);
    }
}

int compare_rows(const void* a, const void* b){
  double *row_a = *((double**) a);
  double *row_b = *((double**) b);
  double result = row_b[0] - row_a[0];
  if (result==0) {
    return 0;
  }
  else if(result > 0){
    return 1;
  }
  else{
    return -1;
  }
}

void sort_matrix_rows(Matrix *matrix){
  qsort(matrix->rows, matrix->num_rows, sizeof(double*), compare_rows);
}

int main () {
    Matrix *matrix = make_matrix(3, 4);
    for (int i=0; i<matrix->num_rows; i++) {
        for (int j=0; j<matrix->num_cols; j++) {
            matrix->rows[i][j] = i + j + 1;
        }
    }

    print_matrix(matrix);
    // printf("reducing...\n");
    // reduce_matrix_rows(matrix, 1, 0);
    // reduce_matrix_rows(matrix, 2, 0);
    // print_matrix(matrix);

    printf("sorting...\n");
    sort_matrix_rows(matrix);
    print_matrix(matrix);

    free_matrix(matrix);
}
