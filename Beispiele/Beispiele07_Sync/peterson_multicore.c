/* 
   V2, 22.11.2019

   compile:
   gcc -o peterson_multicore peterson_multicore.c -lpthread 
*/
#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>


/* choose one or no sync method */
#define SYNC_MFENCE 1
//#define SYNC_ATOMICS  1

#define COUNT      1000000
#define TRUE       1
#define FALSE      0


uint64_t victim  = 0;
uint64_t t1Tries = FALSE;
uint64_t t2Tries = FALSE;

uint64_t  var_sync = 0;
uint64_t  var_asyn = 0;


void *thread_proc1(void *param) {
   pthread_t thread = pthread_self();
   uint64_t zwisch;
   uint64_t count = COUNT;
#ifdef SYNC_ATOMICS
   uint64_t true_v  = 1;
   uint64_t false_v = 0;
   uint64_t vic1    = 1;
#endif
    
   while ( count-- > 0 ) {

      zwisch = var_asyn+1;
     // pthread_yield();   /* release CPU */ 
      var_asyn = zwisch;
   
      /*
         sync entering critical section
      */
      #ifdef SYNC_MFENCE
         t1Tries = TRUE;
         victim  = 1;
         asm("mfence");
      #elif SYNC_ATOMICS
         __atomic_store (&t1Tries, &true_v, __ATOMIC_SEQ_CST);
         __atomic_store (&victim, &vic1, __ATOMIC_SEQ_CST);
      #else
        t1Tries = TRUE;
        victim  = 1;
      #endif

      while (t2Tries==TRUE && victim==1) {};

      zwisch   = var_sync+1;
    //  pthread_yield();   /* release CPU */
      var_sync = zwisch;

      /*
         signal releasing critical section
      */
      #ifdef SYNC_MFENCE
         t1Tries  = FALSE;
         asm("mfence");
     #elif SYNC_ATOMICS
         __atomic_store (&t1Tries, &false_v, __ATOMIC_SEQ_CST);
      #else
         t1Tries  = FALSE;
      #endif
   }

   printf("thread %x, var_asyn: %" PRIu64 "\n", (int)thread, var_asyn);
   printf("thread %x, var_sync: %" PRIu64 "\n", (int)thread, var_sync);
   return NULL;
}


void *thread_proc2(void *param) {
   pthread_t thread = pthread_self();
   uint64_t zwisch;
   uint64_t count = COUNT;
#ifdef SYNC_ATOMICS
   uint64_t true_v  = 1;
   uint64_t false_v = 0;
   uint64_t vic2    = 2;
#endif

   while ( count-- > 0 ) {

      zwisch = var_asyn+1;
   //   pthread_yield();   /* release CPU */ 
      var_asyn = zwisch;


      /*
         sync entering critical section
      */
      #ifdef SYNC_MFENCE
         t2Tries = TRUE;
         victim  = 2;
         asm("mfence");
     #elif SYNC_ATOMICS
         __atomic_store (&t2Tries, &true_v, __ATOMIC_SEQ_CST);
         __atomic_store (&victim, &vic2, __ATOMIC_SEQ_CST);
     #else
        t2Tries = TRUE;
        victim  = 2;
    #endif

      while (t1Tries==TRUE && victim==2) {};

      zwisch = var_sync+1;
   //   pthread_yield();   /* release CPU */ 
      var_sync   = zwisch;


      /*
         signal releasing critical section
      */
      #ifdef SYNC_MFENCE
         t2Tries  = FALSE;
         asm("mfence");
      #elif  SYNC_ATOMICS
         __atomic_store (&t2Tries, &false_v, __ATOMIC_SEQ_CST);
      #else
         t2Tries  = FALSE;
      #endif
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
