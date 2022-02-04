/*
  Compilieren mit: gcc sock_server.c -o sock_server -std=c90
*/

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>			/* Unix Domain Adressen */
#include <unistd.h>
#include <errno.h>


#define UNIXSTR_PATH    	"mySock"
#define BUFF_SIZE		100

int main(int argc, char **argv) {
   int			lSock, cSock, lenAddr, numrcv;
   socklen_t		clilen;
   struct sockaddr_un	cliAddr, servAddr;
   char			buffer[BUFF_SIZE];


   lSock = socket(PF_UNIX, SOCK_STREAM, 0);

   bzero(&servAddr, sizeof(servAddr));
   servAddr.sun_family = AF_UNIX;
   strcpy(servAddr.sun_path, UNIXSTR_PATH);
   lenAddr = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);

   if (bind(lSock, (struct sockaddr *) &servAddr, lenAddr) < 0 )
      printf("error: bind failed with %s\n", strerror(errno));

   if (listen(lSock, 1) < 0)	/* allow max. 5 pending connections */
      printf("error: listen failed with %s\n", strerror(errno));

   clilen = sizeof(cliAddr);
   cSock = accept(lSock, (struct sockaddr *) &cliAddr, &clilen);/* block until connection request  */
   if (cSock<0) 
      printf("error: accept failed with %s\n", strerror(errno));

   numrcv = recv(cSock, buffer, BUFF_SIZE, 0);
   buffer[numrcv] = '\0';
   printf("received %d bytes, data=%s\n", numrcv, buffer);

   close(cSock); 
   close(lSock);
}
