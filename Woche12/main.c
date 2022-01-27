#include "matrix.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define getAddress(matrix_pointer, current_row, current_column, columns) \
    (matrix_pointer + current_row * columns + current_column)
#define RA  2048
#define CA  2048
#define RB  2048
#define CB  2048
int amount = 1;
int progress = 0;

void multiplyMatrices(int64_t* matrixA, int64_t* matrixB, int64_t* matrixC,
                      int rowsA, int colsA, int colsB);

/* Signal auslösen mit: kill –SIGUSR1 pid */
void myHandler(int parm) {
    printf("Progress: %d%%\n",progress*100/amount);
}

int main() {
    struct sigaction action;
    action.sa_flags = 0; /* Standardverhalten für Signale */
    action.sa_handler = &myHandler;
    sigaction(SIGUSR1, &action, NULL); /* Signaltyp, neuer Handler, alter Handler */
    printf("PID ID: %d\n",getpid());
    if (RA != CB || CA != RB) {
        printf("Ungueltige Dimensionen der Matrizen: Die Zeilenanzahl der Matrix A "
               "muss der Spaltenanzahl der Matrix B gleichen und umgekehrt.\n");
        exit(EXIT_FAILURE);
    }

    int64_t* matrixA = malloc(RA * CA * sizeof(int64_t));
    int64_t* matrixB = malloc(RB * CB * sizeof(int64_t));
    int64_t* matrixC = calloc(RA * CB, sizeof(int64_t));

    if (matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("ERROR: Allokation fehlgeschlagen.\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    initializeMatrix(matrixA, RA, CA);
    initializeMatrix(matrixB, RB, CB);

    multiplyMatrices(matrixA, matrixB, matrixC, RA, CA, CB);

    printf("Matrix A:\n");
    printMatrix(matrixA, RA, CA);

    printf("\nMatrix B:\n");
    printMatrix(matrixB, RB, CB);

    printf("\nMatrix C (A mal B):\n");
    printMatrix(matrixC, RA, CB);

    free(matrixA);
    free(matrixB);
    free(matrixC);
    exit(EXIT_SUCCESS);
}

void multiplyMatrices(int64_t* matrixA, int64_t* matrixB, int64_t* matrixC,
                      int rowsA, int colsA, int colsB) {
    amount = rowsA * colsB;
    for (int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                *getAddress(matrixC, i, j, colsB) +=
                        *getAddress(matrixA, i, k, colsA) *
                        *getAddress(matrixB, k, j, colsB);
            }
            progress++;
        }
    }
}