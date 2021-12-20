#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Woche5/matrix.h"
#include "FiFo.h"

void set_prio( pthread_t id, int policy, int prio ) {
    struct sched_param param;
    param.sched_priority = prio;
    if ((pthread_setschedparam( id, policy, &param)) != 0 ) {
        printf("error: unable to change scheduling strategy.\n");
        pthread_exit( (void *) id );
    }
}

void* calculate(void* params) {
    parameters* param = (parameters*) params;
    int64_t* matrixC = calloc(param->rows*param->columns,sizeof(int64_t));
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC,&start);
    multiplyMatrices(param->matrixA,param->matrixB,matrixC,param->rows,param->columns,param->columns);
    clock_gettime(CLOCK_MONOTONIC,&end);
    free(matrixC);
    long* result = (long*) malloc(sizeof(long));
    *result = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000 / 1000);
    return (void*) result;
}

int main() {
    int rows;
    int columns;
    int thread_number;

    printf("rows:\n");
    scanf("%d",&rows);

    printf("columns:\n");
    scanf("%d",&columns);

    printf("threads:\n");
    scanf("%d",&thread_number);

    int64_t* matrixA = malloc(sizeof(int64_t)*rows*columns);
    int64_t* matrixB = malloc(sizeof(int64_t)*rows*columns);

    srand(time(NULL));

    initializeMatrix(matrixA,rows,columns);
    initializeMatrix(matrixB,rows,columns);
    pthread_t* threads = malloc(sizeof(pthread_t)*thread_number);

    parameters* params = (parameters*) malloc(sizeof(parameters));
    params->matrixA = matrixA;
    params->matrixB = matrixB;
    params->columns = columns;
    params->rows = rows;

    for (int i = 0; i < thread_number; ++i) {
        if(pthread_create(&threads[i],NULL,calculate,params)) {
            printf("Thread konnte nicht erstellt werden\n");
            exit(EXIT_FAILURE);
        }
       if(i == thread_number-1) {
           set_prio(threads[i],SCHED_FIFO,99);
       }
    }
    for (int i = 0; i < thread_number; ++i) {
        long* time;
        if(pthread_join(threads[i],(void**)&time)) {
            printf("Thread konnte nicht ausgeführt werden\n");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d finished: %ld ms\n",i,*time);
    }

}