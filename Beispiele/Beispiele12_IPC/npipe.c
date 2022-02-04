/* Compilieren mit: gcc npipe.c -o npipe -std=c90 */
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(void) {
	char *msg1 = "A first message from mother process";
	char *fifo = "myFifo";
	char buffer[64];
	int  fd, status;


	pid_t pid = fork();

	mkfifo(fifo, 0777);

	if (pid > 0) {
		fd = open(fifo, O_WRONLY);
		write(fd, msg1, strlen(msg1)+1);
		wait(&status);	/* wait before deleting file */
	}
	if (pid == 0) {
		fd = open(fifo, O_RDONLY);
		read(fd, buffer, 64);
		printf("%s\n", buffer);
	}
	close(fd);
	unlink(fifo);
}
