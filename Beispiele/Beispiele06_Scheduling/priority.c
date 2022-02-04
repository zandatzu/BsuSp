/* gcc priority.c -lpthread -o priority 
  
   Achtung: Ausgabe funktioniert nur wie gewünscht auf single core
*/
   
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>


#define NR_OF_CHARS 100000

/* volatile: zeigt dem Compiler an, dass diese Variablen thread-übergreifend
             zugegriffen werden und er diese nicht weg-optimieren darf.

   Die beiden Variablen sollen helfen, dass die beiden Threads ungefähr gleichzeitig
   mit ihren Ausgaben beginnen.
*/
volatile int thread1_ready = 0;
volatile int thread2_ready = 0;


void getprio( pthread_t id ) {
   int policy;
   struct sched_param param;
   
   printf("Thread %lx: ", id);
   if((pthread_getschedparam(id, &policy, &param)) == 0  ) {
      printf("scheduling strategy: ");
      switch( policy ) {
         case SCHED_OTHER :  printf("SCHED_OTHER; "); break;
         case SCHED_FIFO  :  printf("SCHED_FIFO; ");  break;
         case SCHED_RR    :  printf("SCHED_RR; ");    break;
         default          :  printf("Unbekannt; ");  break;
      }
      printf("priority: %d\n", param.sched_priority);
   }
}


void setprio( pthread_t id, int policy, int prio ) {
   struct sched_param param;

   param.sched_priority = prio;

   if((pthread_setschedparam( id, policy, &param)) != 0  ) {
      printf("error: could not change scheduling strategy. May be not root?\n");
      pthread_exit((void *)id);
   }
}


/* second thread */
void *my_thread(void *param) {
   char ch = *( (char *)param);
   
   getprio(pthread_self());
   setprio(pthread_self(), SCHED_FIFO, 1);
   getprio(pthread_self());

   while(thread1_ready==0) ;  /* warte bis der Haupt-Thread bereit ist */
   thread2_ready = 1;  /* signalisiere dem Haupt-Thread, dass Thread-2 startet */

   for (int i=0; i<NR_OF_CHARS; i++)
      printf(ch);
  
   return NULL;
}


int main() {
   pthread_t thread_ref; /* thread reference */
   char ch1 = '*';
   char ch2 = '.';
   int i,ret;
 

   /* create another thread */
   if ( pthread_create(&thread_ref, NULL, &my_thread, &ch2) ) {
      printf("error: creating thread failed.\n");
      return EXIT_FAILURE;
   }
  

   /* changing strategy and priority of main thread */
   /* (reference, attributes (priority, stacksize, ...), function, params) */   
   getprio(pthread_self());
   setprio(pthread_self(), SCHED_FIFO, 99);
   getprio(pthread_self());

   thread1_ready = 1;  /* signalisiere Thread-2, dass Haupt-Thread bereit ist */
   while(thread2_ready==0) ; /* warte auf Thread-2 */

   /* print characters */
   for (int i=0; i<NR_OF_CHARS; i++)
      printf(ch1); 

   /* wait for the second thread to finish */
   if (pthread_join(thread_ref, NULL)) {
      printf("error: join failed.\n"); 
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
