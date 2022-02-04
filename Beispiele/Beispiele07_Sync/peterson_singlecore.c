
/*
   V2, 22.11.2019
 
   compile:
   gcc -o peterson_singlecore peterson_singlecore.c -lpthread
*/
#define _GNU_SOURCE      /* See feature_test_macros(7) */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

#define PINNING    /* pin threads on core0 */

#define COUNT   1000000
#define DUMMY   1000     /* count for dummy loop */
#define TRUE    1
#define FALSE   0


int victim  = 0;
int t1Tries = FALSE;
int t2Tries = FALSE;


uint64_t  var_sync = 0;
uint64_t  var_asyn = 0;


/* pin calling thread to core0 */
void pin_thread_on_core0() {
   int s;
   cpu_set_t cpuset;
   pthread_t thread = pthread_self();

   CPU_ZERO(&cpuset);   /* clear set */
   CPU_SET(0, &cpuset); /* set core0 */

   /* Set affinity mask for thread */
   if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
      printf("ERROR: pthread_setaffinity_np failed\n");
      exit(EXIT_FAILURE);
   }
   
   /* Check the actual affinity mask assigned to the thread */
   if ( pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
      printf("ERROR: pthread_getaffinity_np failed.\n");
      exit(EXIT_FAILURE);
   }
   printf("Pinned thread %x on core0\n", (int)thread);
   if (pthread_yield() != 0) {
      printf("ERROR: pthread_yield failed.\n");
      exit(EXIT_FAILURE);
   }
}


void *thread_proc1(void *param) {
   pthread_t thread = pthread_self();
   uint64_t zwisch;
   uint64_t count = COUNT;

#ifdef PINNING
   pin_thread_on_core0();
#endif

   while ( count-- > 0 ) {
      zwisch = var_asyn+1; var_asyn = zwisch;

      /* sync entering critical section */
      t1Tries = TRUE;
      victim  = 1;
      while (t2Tries==TRUE && victim==1) {};

      zwisch   = var_sync+1;
      var_sync = zwisch;

      /* signal releasing critical section */
      t1Tries = FALSE;

     /* consume up some time */
      for (int i=0; i<DUMMY; i++) ;
   }

   printf("thread %x, var_asyn: %" PRIu64 "\n", (int)thread, var_asyn);
   printf("thread %x, var_sync: %" PRIu64 "\n", (int)thread, var_sync);
   return NULL;
}


void *thread_proc2(void *param) {
   pthread_t thread = pthread_self();
   uint64_t zwisch;
   uint64_t count = COUNT;

#ifdef PINNING
   pin_thread_on_core0();
#endif

   while ( count-- > 0 ) {
      zwisch = var_asyn+1;  var_asyn = zwisch;


      /* sync entering critical section */
      t2Tries = TRUE;
      victim  = 2;
      while (t1Tries==TRUE && victim==2) {};

      zwisch   = var_sync+1;
      var_sync = zwisch;

      /* signal releasing critical section */
      t2Tries = FALSE;

     /* consume up some time */
      for (int i=0; i<DUMMY; i++) ;
  }

   printf("thread %x, var_asyn: %" PRIu64 "\n", (int)thread, var_asyn);
   printf("thread %x, var_sync: %" PRIu64 "\n", (int)thread, var_sync);
   return NULL;
}


int main() {
   pthread_t tref[2]; /* thread references */
   int i;

   /* create first thread */
   if ( pthread_create(&tref[0], NULL, &thread_proc1, NULL) ) {
      printf("ERROR: pthread_create failed.\n");
      return EXIT_FAILURE;
   }
   /* create second thread */
   if ( pthread_create(&tref[1], NULL, &thread_proc2, NULL) ) {
      printf("ERROR: pthread_create failed.\n");
      return EXIT_FAILURE;
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

