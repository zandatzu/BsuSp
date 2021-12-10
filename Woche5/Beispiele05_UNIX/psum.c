/* Hörsaal-Aufgabe */

#include <stdio.h>
#include <string.h>
#include <unistd.h>		/* different standard constants 		*/
#include <stdlib.h>

int main (int argc, char **argv) {
   pid_t  pid;
   char   data[80];
   int    rb;            /* nr of read bytes    */
   int    wb;            /* nr of written bytes */
   int    pipe_ends[2];  /* handles: read=0; write=1 */
   int    i, number=0;

   if (argc<2) {
      printf("ERROR: number expected.\n");
      exit(EXIT_FAILURE);
   }

    if (pipe(pipe_ends) == -1) {        /* create anonymous pipe */
       printf("ERROR: pipe() failed.\n");
       return EXIT_FAILURE;
    }

    pid = fork ();
    if (pid == (pid_t)-1 ) {
       printf ("ERROR: fork failed.\n");
       return EXIT_FAILURE;
    }
    else if ( pid == (pid_t)0 ) { /* child process */
      close(pipe_ends[0]);	/* close read end -> child wants to write	*/

      for (i=0; i<strlen(argv[1]); i++) {
         number += ((int)argv[1][i] - 48);
      }

/*
      // Loesung 1
      sprintf(data, "%d", number);
      write(pipe_ends[1], data, strlen(data));
*/

      // Loesung 2
      write(pipe_ends[1], &number, sizeof(number) );

      close(pipe_ends[1]);	/* done, close write end			*/
   }
   else { 			/* parent process				*/
      close(pipe_ends[1]);	/* close read end -> process wants to writen	*/

/*
      // Loesung 1
      rb = read(pipe_ends[0], data,79);
      data[rb]='\0';
      printf ("cross sum of %s = %s\n", argv[1], data);
*/

      // Loesung 2
      int ergebnis;
      read(pipe_ends[0], &ergebnis,sizeof(ergebnis));
      printf ("cross sum of %s = %d\n", argv[1], ergebnis);

      close(pipe_ends[0]); 	/* done, close write end			*/
   }
   return EXIT_SUCCESS;
}
