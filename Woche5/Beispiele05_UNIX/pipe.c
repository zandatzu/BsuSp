/* Beispiel: Anonyme Pipe zwischen Eltern- und Kind-Prozess */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* different standard constants */

int main() {
   pid_t  pid;
   char   data[80];
   int    rb;           /* nr of read bytes */
   int    wb;           /* nr of written bytes */
   int    pipe_ends[2];	/* handles: read=0; write=1 */


   if (pipe(pipe_ends) == -1) {		/* create anonymous pipe */
      printf("ERROR: pipe() failed.\n");
      return EXIT_FAILURE;
   }

   pid = fork ();
   if (pid == (pid_t)-1 ) {
      printf ("ERROR: fork failed.\n");
      return EXIT_FAILURE;
   }
   else if ( pid == (pid_t)0 ) { /* child process */
      close(pipe_ends[1]);	/* close write end -> process wants to read */
      rb = read(pipe_ends[0], data,79);
      if (rb==-1) {
         printf("ERROR: read() failed.\n");
         return EXIT_FAILURE;
      }
      data[rb]='\0';		/* terminate string	*/
      printf ("%s\n",data);
      close(pipe_ends[0]);	/* done, close read end	*/
   }
   else { 				/* parent process */
      close(pipe_ends[0]);	/* close read end -> process wants to writen */
      wb=write(pipe_ends[1],"hello",5);
      if (wb==-1) {
         printf("ERROR: write() failed.\n");
         return EXIT_FAILURE;
      }
      close(pipe_ends[1]); 	/* done, close write end */
   }
   return EXIT_SUCCESS;
}
