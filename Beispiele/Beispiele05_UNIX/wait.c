/* Beispiel zu waitpid */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main () {
   pid_t pid;
   int status;

   pid = fork ();
   if (pid == (pid_t)-1 ) {
       printf ("ERROR: fork failed.\n");
       return EXIT_FAILURE;
   }
   else if ( pid == (pid_t)0 ) {
      printf ("Child: PID: %d, sleeping.\n", getpid());
      sleep(5);
      printf ("Child: done.\n");
      exit(EXIT_SUCCESS);
   }
   else {
      printf ("Parent: PID: %d, waiting for child ...\n", getpid());
      waitpid(pid, &status, 0);
      printf("Parent: child returned %d\n", status);
      printf("Parent: done.\n");
   }
   return EXIT_SUCCESS;
}
