#include <stdio.h>
#include <stdlib.h>

#define NR_OF_ENTRIES   5

int array[] = { 88, 56, 100, 2, 25 };

int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void dump() {
    int i;

    printf("The array: ");
    for(i=0; i<NR_OF_ENTRIES; i++ ) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main () {
    dump();
    qsort(array, NR_OF_ENTRIES, sizeof(int), &cmpfunc);
    dump();
    return 0;
}
