#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long long shared_sum = 0;

typedef struct parameters {
    int* arr;
    int offset;
    int length;
} param_t;

void fill_array(int* arr, int length) {
    for (int i = 0; i < length; i++) {
        *(arr + i) = random() % 100000;
    }
}

void* get_sum_parallel(void* data) {
    long long sum = 0;
    int *arr = ((param_t*) data)->arr;
    int offset = ((param_t*) data)->offset;
    int length = ((param_t*) data)->length;

   for (int i = offset; i < offset + length; i++) {
        int value = *(arr + i);
        sum += value;
    }

    pthread_mutex_lock(&mutex);
    shared_sum += sum;
    pthread_mutex_unlock(&mutex);

    free(data);

    return (void*) sum;
}

int main (int argc, const char* argv[]) {
    if (argc != 3) {
        printf("Usage: sum <array length> <number of threads>\n");
        exit(EXIT_FAILURE);
    }

    long array_length = strtol(argv[1], NULL, 10);
    if (array_length < 0 || array_length > INT_MAX
            || array_length & (array_length - 1)) {
        printf("Invalid array length: %ld\n", array_length);
        exit(EXIT_FAILURE);
    }
    long threads = strtol(argv[2], NULL, 10);
    if (threads < 0 || threads > 16 || threads & (threads - 1)) {
        printf("Invalid number of threads: %ld\n", threads);
        exit(EXIT_FAILURE);
    }

    printf("Initializing...\n");
    srand(time(NULL));

    int* arr = (int*) malloc(sizeof(int) * array_length);
    if (arr == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    fill_array(arr, array_length);

    printf("Starting...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t pthreads[threads];
    for (int i = 0; i < threads; i++) {

        param_t* data = (param_t*) malloc(sizeof(param_t));
        data->arr = arr;
        data->offset = i * (array_length / threads);
        data->length = array_length / threads;

        if (pthread_create(&pthreads[i], NULL, get_sum_parallel, data) != 0) {
            printf("Could not create threads\n");
            exit(EXIT_FAILURE);
        }
    }

    long long sum = 0;
    void* ret_ptr = NULL;
    for (int i = 0; i < threads; i++) {
        pthread_join(pthreads[i], &ret_ptr);
        sum += (long long) ret_ptr;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("Sum: %lld; Shared Sum: %lld; determined in %ld ms\n", sum, shared_sum,
        (end.tv_sec - start.tv_sec) * 1000
        + (end.tv_nsec - start.tv_nsec) / 1000 / 1000);

    free(arr);
}