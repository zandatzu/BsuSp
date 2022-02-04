#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define MAXMYMEM 	30
#define SHM_KEY		2404

int main(int argc, char **argv) {
	int shID;
	char *myPtr;
	int i;

    	shID = shmget(SHM_KEY, MAXMYMEM, 0666);
    	if (shID >= 0) {
        	myPtr = shmat(shID, 0, 0);
        	if (myPtr==(char *)-1)
            		printf("error: shmat failed\n");
        	else {
            		for (i=0; i<MAXMYMEM; i++) 
                		putchar(myPtr[i]);
            		puts("\n");
            		shmdt(myPtr);
        	}
    	} else 
        	printf("error: shmget failed\n");
}

