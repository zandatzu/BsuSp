#include <stdio.h>

int main() {
    int array[] = { 45, 67, 89, 0 };
    int *ptr = array;
    
    while (*ptr != 0) {
        printf("ptr=%p, %d\n", array_ptr, *ptr );
        // falsch: ptr += sizeof(int);
        ptr++;
    }
}
