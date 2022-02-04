/* gcc -m32 -o aslr-demo aslr-demo.c */

#include <stdio.h> 
#include <malloc.h> 

long data = 16; 
long bss;

int main() {
   long stack;
   long *heap = malloc(8); 
   printf("Text : %08x\n", (unsigned int) &main); 
   printf("Data : %08x\n", (unsigned int) &data); 
   printf("BSS  : %08x\n", (unsigned int) &bss); 
   printf("Heap : %08x\n", (unsigned int) heap); 
   printf("Stack: %08x\n", (unsigned int) &stack);
}
