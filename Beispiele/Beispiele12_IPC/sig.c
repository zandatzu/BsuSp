/*
   Compilieren mit: gcc sig.c -o sig -std=c90
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* Signal auslesen mit: kill -SIGUSR1 pid */

void myHandler(int parm) {
 	printf("received SIGUSR1\n");
}

int main() {
 	struct sigaction action,old;

	action.sa_flags=0;
	sigemptyset(&action.sa_mask);
	action.sa_handler = &myHandler;

	sigaction(SIGUSR1, &action,  &old);	/* Signaltyp, neuer Handler, alter Handler */
        printf("pid=%d\n", getpid());

	sleep(60); 				/* 60s schlafen;  unterbrechbar durch Signal! */

	sigaction(SIGUSR1, &old, NULL);
}
