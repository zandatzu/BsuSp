#include <stdio.h>
#include <string.h>

union Data {
    char ch;
    int i;
    char str[20];
};

int main( ) {
    union Data data;
    
    printf( "sizeof union : %lu\n", sizeof(data));
    
    data.ch = 'A';
    data.i  = 10;
    strcpy( data.str, "C programming");
    
    printf( "data.ch : %c\n", data.ch);
    printf( "data.i : %d\n", data.i);
    printf( "data.str : %s\n", data.str);
    

    printf("data.ch: %c\n", data.ch);
    return 0;
}
