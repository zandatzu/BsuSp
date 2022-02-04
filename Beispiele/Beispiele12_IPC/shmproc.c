/* shared memory between parent and child process */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <wait.h>


#define MAXMYMEM 	4096
#define PATH		"/home/student"
#define PROJ_ID 	4711


int main () {
    pid_t   pid;
    key_t  key;
   	int    shID;
    int    status;

    key = ftok(PATH, PROJ_ID);
    if (key < 0) {
       printf("parent-error: ftok failed.\n");
       exit(EXIT_FAILURE);
    }

    shID = shmget(key, MAXMYMEM, IPC_CREAT | 0666);
    if (shID < 0) {
       printf("parent-error: shmget failed.\n");
       exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) { 
      printf("parent-error: fork failed.\n");
      exit(EXIT_FAILURE);
    }

    if (pid == (pid_t) 0) { 
        printf("child:  running\n");

        char *str   = "Hello World!";
	    char *myPtr = shmat(shID, 0, 0);
        if (myPtr == (char*)-1) {
           printf("child-error: shmat failed.\n");
           exit(EXIT_FAILURE);
        }

        printf("child:  writing |%s| at address = %x\n", str, (uint32_t)myPtr);
        strcpy(myPtr, str);
 		
        shmdt(myPtr);
        printf("child:  finished\n");
        return 0;
    }
    
    printf("parent: waiting for child\n");
    waitpid(pid, &status, 0);

    char *myPtr = shmat(shID, 0, 0);
    if (myPtr == (char*)-1) {
       printf("parent-error: shmat failed.\n");
       exit(EXIT_FAILURE);
    }
    printf("parent: ptr to shared memory = %x\n", (uint32_t)myPtr);
    printf("parent: data from child = |%s|\n", myPtr);

    shmdt(myPtr);
    shmctl(shID, IPC_RMID, 0);
        
    printf("parent: finished.\n");

    return 0;
}
