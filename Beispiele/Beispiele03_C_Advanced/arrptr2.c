#include <stdio.h>

char *a = "A string";
char b[] = "Another string";

int main (void) {

    *a = '*';   /* error -> writing in string constant */        
    
    a = b;
    *a = '*';   /* OK -> writing to array now */
    b = a;      /* erlaubt Compiler nicht */
    
    printf("%s\n", b);
    return 0;
}

