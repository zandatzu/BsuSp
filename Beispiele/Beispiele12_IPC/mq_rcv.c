/*
   Compilieren mit: gcc mq_rcv.c -o mq_rcv -std=c90
*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSGSIZE 	20
#define MQ_KEY		2404

struct myMsg {
  long mtype;
  char mtext[MSGSIZE];
} dataMsg;

int main(int argc, char **argv)
{
	int  msgID;
	long msgTyp = 0;

      	if (argc<2) {
		printf("error: mq_snd mtype\n");
		return -1;
	}

       	msgTyp = atol(argv[1]);

      	msgID = msgget(MQ_KEY, IPC_CREAT | 0666);
	if (msgID >= 0) {
        	printf("waiting for message of type %ld\n", msgTyp);
        	if (msgrcv(msgID, &dataMsg, MSGSIZE, msgTyp, 0)<0) {
           		printf("error: msgrcv failed\n");
        	} else {
            		printf("message received: %s\n", dataMsg.mtext);
        	}
    	} else {
        	printf("error: msgget failed\n");
    	}
}

