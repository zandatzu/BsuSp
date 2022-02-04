/* Beispiel: Zombie */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
   pid_t pid;
   int status;

   pid = fork ();

   if (pid == (pid_t)-1 ) {
       printf ("ERROR: fork failed.\n");
       return EXIT_FAILURE;
   }
   else if ( pid == (pid_t)0 ) {
      printf ("Child: PID: %d, done.\n", getpid());
      exit(EXIT_SUCCESS);
   }
   else {
      printf ("Parent: PID: %d, sleeping ...\n", getpid());
      sleep(60);
      printf("Parent: done.\n");
   }
   return EXIT_SUCCESS;
}
