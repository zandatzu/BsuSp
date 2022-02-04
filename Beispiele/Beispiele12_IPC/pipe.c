#include <stdio.h>
#include <unistd.h>

int main() {
	char 	data[80];
	int	rb;			/* nr of read bytes		*/
	int 	pipe_ends[2];		/* handles: read=0; write=1	*/

	pipe(pipe_ends);
	if (fork()==0) { 		/* child process		*/
		close(pipe_ends[1]);	/* close write end		*/
		rb = read(pipe_ends[0], data,79);
 		data[rb]='\0';		/* terminate string		*/
		printf ("%s\n",data);
		close(pipe_ends[0]);	/* close read end		*/
	}
	else { 				/* father process		*/
		close(pipe_ends[0]);	/* close read end		*/
		write(pipe_ends[1], "hello from parent process", 25);
		close(pipe_ends[1]); 	/* close write end		*/
	}
}
