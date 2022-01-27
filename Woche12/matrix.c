#include <stdlib.h>  // Fuer int64_t
#include "matrix.h"
#include <stdio.h>

#define getAddress(matrix_pointer, current_row, current_column, columns) \
    (matrix_pointer + current_row * columns + current_column)

void initializeMatrix(int64_t* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *getAddress(matrix, i, j, cols) = rand() % 100;
        }
    }
}

void printMatrix(int64_t* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%ld ", *getAddress(matrix, i, j, cols));
        }
        printf("\n");
    }
}