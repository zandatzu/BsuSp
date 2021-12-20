#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int* array;
    int length;
}parameters;

void* maximum(void *params) {
    parameters* param = (parameters*) params;
    int max = param->array[0];
    for (int i = 0; i < param->length; ++i) {
        if(param->array[i] > max) {
            max = param->array[i];
        }
    }
    int* result = malloc(sizeof(int));
    *result = max;
    return result;
}


int main() {
    int array[256];
    srand(time(NULL));
    for (int i = 0; i < 255; ++i) {
        *(array+i) = (rand() % 100);
    }
    int number_threads;
    printf("Geben Sie die Anzahl an Threads ein:\n");
    scanf("%d",&number_threads);
    pthread_t* threads = malloc(sizeof(pthread_t)*number_threads);
    parameters* params = malloc(sizeof(parameters)*number_threads);
    int length = 256 / number_threads;
    int* start = array;
    for (int i = 0; i < number_threads; ++i) {
        params[i].length = length;
        params[i].array = start;
        start+=length;
    }
    for (int i = 0; i < number_threads; ++i) {
        if(pthread_create(&threads[i],NULL,maximum,&params[i])) {
            printf("Thread konnte nicht erstellt werden\n");
            exit(EXIT_FAILURE);
        }
    }
    int max = 0;

    for (int i = 0; i < number_threads; ++i) {
        int* result;
        if(pthread_join(threads[i],(void**)&result)) {
            printf("Thread konnte nicht ausgeführt werden\n");
            exit(EXIT_FAILURE);
        }
        if(*result > max) {
            max = *result;
        }
    }
    free(threads);
    free(params);
    printf("Das Maximum des Arrays ist %d\n",max);
    return 0;
}