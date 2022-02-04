#include <stdio.h>

#define DIM0    4
#define DIM1    2

void dump(char array[DIM0][DIM1]) {
    int  x,y;

    for (y=0; y<DIM0; y++) {
        for (x=0; x<DIM1; x++) {
            printf("%c", array[y][x]);
        }
        printf("\n");
    }
}


int main() {
    char array[DIM0][DIM1];
    char *array_ptr = array[0];
    char ch = 'A';
    int  i;

    for (i=0; i<DIM0*DIM1; i++) {
        *array_ptr++ = ch++;
    }
    
    dump(array);
    
    return 0;
}
