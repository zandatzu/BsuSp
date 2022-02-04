/* gcc thread.c -lpthread -o thread */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT  100000

/* second thread */
void *my_thread(void *param) {
   char ch = *( (char *)param);
   int i,number;


   /* print characters */
   for (i=0; i<COUNT; i++) {
      putchar(ch);
   }

   return NULL;
}


int main() {
   pthread_t thread_ref; /* thread reference */
   char ch1 = '*';
   char ch2 = '.';
   int i;

   /* create another thread */
   /* (reference, attributes (priority, stacksize, ...), function, params) */
   if ( pthread_create(&thread_ref, NULL, &my_thread, &ch2) ) {
      fprintf(stderr, "Error creating thread\n");
      return EXIT_FAILURE;
   }

   /* print characters */
   for (i=0; i<COUNT; i++) {
      putchar(ch1);
   }

   /* wait for the second thread to finish */
   if (pthread_join(thread_ref, NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
