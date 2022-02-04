#include <stdio.h>

int main(void) {
    int array[] = { 1, 2, 3 };
    int *ptr;
    int i;
    
    ptr = &array[0];
    
    for (i=0; i<3; i++) {
        printf("%d. element: %d (%d)\n", i, array[i], *(ptr+i) );
    }
}
