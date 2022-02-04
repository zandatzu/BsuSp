/* gcc -mpush-args -m32 –fno-pie -o stack stack.c */

#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int e;
    
    e = add(4,2);
    return 0;
}
