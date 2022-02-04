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

    	shID = shmget(SHM_KEY, MAXMYMEM, IPC_CREAT | 0666);
    	if (shID >= 0) {
		myPtr = shmat(shID, 0, 0);
        	if (myPtr==(char *)-1) 
            		printf("error: shmat failed\n");
        	else {
            		for (i=0; i<MAXMYMEM; i++) {
                		myPtr[i] = 'A'+i;
            		}
			printf("data written in shared memory, waiting for key\n");
            		getchar(); 
                     	shmdt(myPtr);
        	}
 		shmctl(shID, IPC_RMID, 0);
    	} else 
        	printf("error: shmget failed\n");
}

