/* gcc sync123.c -o sync123 -lpthread */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;


void *print_1 (void *dummy) {
   char ch = '1';

   while(1) {
      pthread_mutex_lock( &mutex1 );

      putchar(ch);
      fflush(stdout);

      pthread_mutex_unlock( &mutex2 );
  }

  return NULL;
}

void *print_2 (void *dummy) {
   char ch = '2';

   while(1) {
      pthread_mutex_lock( &mutex2 );

      putchar(ch);
      fflush(stdout);

      pthread_mutex_unlock( &mutex3 );
   }

  return NULL;
}


void *print_3 (void *dummy) {
   char ch = '3';

   while(1) {
      pthread_mutex_lock( &mutex3 );

      putchar(ch);
      fflush(stdout);

      pthread_mutex_unlock( &mutex1 );
   }

  return NULL;
}



int main () {
   pthread_t tref[3];
   int i;


   // Thread 2 und 3 sollen zunächst blockieren
   pthread_mutex_lock( &mutex2 );
   pthread_mutex_lock( &mutex3 );

   if (pthread_create (&tref[0], NULL, print_1, NULL)) {
      fprintf(stderr, "error: creating thread 1 failed.\n");
      return EXIT_FAILURE;
   }

   if (pthread_create (&tref[1], NULL, print_2, NULL)) {
      fprintf(stderr, "error: creating thread 2 failed.\n");
      return EXIT_FAILURE;
   }
   if (pthread_create (&tref[2], NULL, print_3, NULL)) {
      fprintf(stderr, "error: creating thread 3 failed.\n");
      return EXIT_FAILURE;
   }
 

   for (i=0; i<3; i++) { 
      if (pthread_join (tref[i], NULL)) {
          fprintf(stderr, "error: joining thread %d failed.\n", i);
          return EXIT_FAILURE;
      }
   }

   return EXIT_SUCCESS;
}
