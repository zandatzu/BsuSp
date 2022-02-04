#define _GNU_SOURCE /* needed for clone, must be before sched.h */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

   
int thread_proc(void *arg) {
  	/* Endlosschleife für zweiten Thread */
  	for(;;) putchar('*');
}

int main() {
   char *stack = (char*)malloc(4096);

   /* int clone(int (*fn)(void*), void *stack, int flgs, void *arg) 		*/;
   /* stack wächst nach unten Start amd Ende des Arrays “+4000” 	*/
   if (clone(&thread_proc, stack+4096, CLONE_VM, 0) < 0) {
       printf ("ERROR: clone failed.\n");
       return EXIT_FAILURE;
   }
   
   /* Endlosschleife für Hauptthread */
   for(;;) putchar('.');

   free(stack);
   return EXIT_SUCCESS;
} 
