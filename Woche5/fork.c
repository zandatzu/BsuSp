#include <stdio.h>   // Fuer printf, scanf
#include <stdlib.h>  // Fuer exit, rand, srand
#include <string.h>  // Fuer
#include <time.h>    // Fuer time
#include <unistd.h>  // Fuer fork, sleep
#include <wait.h>    // Fuer waitpid
#include "matrix.h"

int main () {
    int rows, columns;
    pid_t pid;
    int status;

    printf("Bitte Anzahl der Zeilen eingeben: ");
    if (scanf("%d", &rows) != 1 || rows < 1) {
        printf("ERROR: Dies ist keine natuerliche Zahl.\n");
        exit(EXIT_FAILURE);
    }
    printf("Bitte Anzahl der Spalten eingeben: ");
    if (scanf("%d", &columns) != 1 || columns < 1) {
        printf("ERROR: Dies ist keine natuerliche Zahl.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == (pid_t)0) {
        int64_t* matrixA = malloc(rows * columns * sizeof(*matrixA));
        int64_t* matrixB = malloc(rows * columns * sizeof(int64_t));
        int64_t* matrixC = calloc(rows * columns, sizeof(int64_t));

        if (matrixA == NULL || matrixB == NULL || matrixC == NULL) {
            printf("ERROR: Allokation fehlgeschlagen.\n");
            exit(EXIT_FAILURE);
        }
        srand(time(NULL));
        initializeMatrix(matrixA, rows, columns);
        initializeMatrix(matrixB, rows, columns);
        multiplyMatrices(matrixA,matrixB,matrixC,rows,columns,columns);
        free(matrixA);
        free(matrixB);
        free(matrixC);
        exit(0);
    }
    else if(pid != (pid_t)-1) {
        int seconds = 0;
        int status = -1;
        printf("0\n");
        while(waitpid(pid,&status,WNOHANG) == 0) {
            sleep(1);
            seconds++;
            printf("%d\n",seconds);
        }
        exit(0);
    }
}

