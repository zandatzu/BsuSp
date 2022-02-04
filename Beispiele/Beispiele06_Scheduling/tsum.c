/* gcc tsum.c -lpthread -o tsum */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


/* second thread */
void *my_thread(void *param) {
   char *zahl_str = (char *)param; /* copy param to our own stack */
   char *data = malloc(80); /* return data -> on heap */
   int i, number=0;


   for (i=0; i<strlen(zahl_str); i++) {
      number += ((int)zahl_str[i] - '0');
   }
   sprintf(data, "%d", number);
//   data[i] = '\0';

   return data;
}


int main (int argc, char **argv) {
   pthread_t thread_ref; /* thread reference */
   char *ret; /* pointer to string returned by 2nd thread */


   if (argc<2) {
      printf("error: number expected.\n");
      return EXIT_FAILURE;
   }

   /* create another thread */
   /* (reference, attributes (priority, stacksize, ...), function, params) */
   if ( pthread_create(&thread_ref, NULL, &my_thread, argv[1]) ) {
      fprintf(stderr, "Error creating thread\n");
      return EXIT_FAILURE;
   }

  /* wait for the second thread to finish */
   if (pthread_join(thread_ref, (void**)&ret)) {
      fprintf(stderr, "Error joining thread\n");
      return EXIT_FAILURE;
   }

   printf ("cross sum of %s = %s\n", argv[1], ret); 

   free(ret);
   return EXIT_SUCCESS;
}
