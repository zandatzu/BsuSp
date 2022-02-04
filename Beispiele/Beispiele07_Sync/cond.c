/* gcc cond.c -o cond -lpthread */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define NR_OF_THREADS  3
#define NR_OF_SECS     5


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;


void *threads (void *arg) {
   if (pthread_mutex_lock(&mutex) != 0) {
        printf("ERROR: pthread_mutex_lock failed.\n");
        exit(EXIT_FAILURE);
   }
   printf("\tThread %ld waiting for condition.\n", pthread_self() );

   if (pthread_cond_wait(&cond, &mutex) != 0) {
        printf("ERROR: pthread_cond_wait failed.\n");
        exit(EXIT_FAILURE);
   }
   
   printf("\tThread %ld received signal.\n", pthread_self());
   printf("\tThread %ld: signal for others.\n", pthread_self());

   if (pthread_cond_signal(&cond) != 0) {
        printf("ERROR: pthread_cond_signal failed.\n");
        exit(EXIT_FAILURE);
   }
   if (pthread_mutex_unlock(&mutex) != 0) {
        printf("ERROR: pthread_mutex_unlock failed.\n");
        exit(EXIT_FAILURE);
   }
   return NULL;
}


int main (void) {
   int i;
   pthread_t tref[NR_OF_THREADS];


   printf("Main-thread: started.\n");
   for(i=0; i<NR_OF_THREADS; i++) {
      if (pthread_create (&tref[i],NULL, &threads, NULL)!=0) {
         printf("ERROR: pthread_create failed.\n");
         return EXIT_FAILURE;
      }
   }
   printf("Main-thread: created %d threads\n", NR_OF_THREADS);
   printf("Main-thread: waiting %d seconds\n", NR_OF_SECS);

   /* wait for the other threads to start */
   sleep(NR_OF_SECS);

   printf("Main-thread: signal condition.\n");
   if (pthread_cond_signal(&cond) != 0) {
        printf("ERROR: pthread_cond_signal failed.\n");
        return EXIT_FAILURE;
   }
 
   printf("Main-thread: waiting for all threads to finish.\n");
   for(i=0; i<NR_OF_THREADS; i++) {
      if (pthread_join(tref[i], NULL)!=0) 
         printf("ERROR: pthread_join failed.\n");
         return EXIT_FAILURE;
      }
      
   printf("Main-thread: exiting.\n");
   return EXIT_SUCCESS;
}

