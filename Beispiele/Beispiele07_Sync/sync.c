/* gcc sync.c -o sync -lpthread */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define COUNT  1000000


long sync=0;
long asyn=0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/* worker threads */
void *my_thread(void *param) {
  long zwisch;		// force non atomic
  int count = COUNT;

   while ( count-- > 0 ) {
      zwisch=asyn+1; asyn=zwisch;
      
      if (pthread_mutex_lock( &mutex ) != 0) {
        printf("ERROR: pthread_mutex_lock failed.\n");
        exit(EXIT_FAILURE);
      }
      
      zwisch = sync+1; 
      sync   = zwisch;
      
      if (pthread_mutex_unlock( &mutex ) != 0) {
        printf("ERROR: pthread_mutex_unlock failed.\n");
        exit(EXIT_FAILURE);
      }
   }

   printf("asyn: %ld\n", asyn);
   printf("sync: %ld\n", sync);
   return NULL;
}


int main() {
   pthread_t tref[2]; /* thread references */
   int i;

   /* create two worker threads */
   for (i=0; i<2; i++) {
      if ( pthread_create(&tref[i], NULL, &my_thread, NULL) ) {
         printf("ERROR: pthread_create failed.\n");
         return EXIT_FAILURE;
      }
   }

   /* wait for all workers to finish */
   for (i=0; i<2; i++) {
      if (pthread_join(tref[i], NULL)) {
         printf("ERROR: pthread_join failed.\n");
         return EXIT_FAILURE;
      }
   }

   return EXIT_SUCCESS;
}
