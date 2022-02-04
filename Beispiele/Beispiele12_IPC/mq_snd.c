/*
   Compilieren mit: gcc mq_snd.c -o mq_snd -std=c90
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

      	if (argc<3) {
		printf("error: mq_rcv mtype msg\n");
		return -1;
	}

        dataMsg.mtype = atol(argv[1]);
        strncpy(dataMsg.mtext, argv[2], MSGSIZE);

     	msgID = msgget(MQ_KEY, IPC_CREAT | 0666);
    	if (msgID >= 0) {
        	printf("sending messagetyp %ld, text: %s\n", dataMsg.mtype, dataMsg.mtext);
        	if (msgsnd(msgID, &dataMsg, MSGSIZE, 0)<0) {
       			printf("error: msgsnd failed");
        	} else {
			printf("data sent\n");
        	}
    	} else {
        	printf("error: msgget failed\n");
    	}
}

