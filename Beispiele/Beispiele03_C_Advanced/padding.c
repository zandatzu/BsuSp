/*  gcc -m32 -O1 padding.c */
#include <stdio.h>

struct SomeData {
    char  c1;
    char  c2;
    int   i;
}foo;
//}__attribute__((packed)) foo;


int main() {
    char *ptr = (char*) &foo;
    
    foo.c1 = 'a';
    foo.c2 = 'b';
    foo.i  = 0xAABBCCDD;
    
    printf("size = %lu\n", sizeof(foo));
    
    return 0;
}
