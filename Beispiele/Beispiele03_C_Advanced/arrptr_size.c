#include <stdio.h>

void foo(int array[], unsigned int size) {
    printf("sizeof(array)=%lu, size=%u\n", sizeof(array), size );
}

int main(void) {
    int array[] = { 1, 2, 3, 4, 5 };
    
    printf("sizeof(array)=%lu\n", sizeof(array) );
    
    foo( array, 3 );
}

