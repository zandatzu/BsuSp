/*
   Compilieren mit: gcc sock_client.c -o sock_client -std=c90
*/

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>		/* Unix Domain Adressen */
#include <unistd.h>

#define UNIXSTR_PATH   "mySock"

int main(int argc, char **argv) {
   int			cSock,lenAddr,s;
   struct sockaddr_un	servAddr;
   char 		*buffer = "hello world";

   cSock = socket(PF_UNIX, SOCK_STREAM, 0);

   bzero(&servAddr, sizeof(servAddr));
   servAddr.sun_family = AF_UNIX;
   strcpy(servAddr.sun_path, UNIXSTR_PATH);
   lenAddr = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);

   connect(cSock, (struct sockaddr *) &servAddr, lenAddr);

   s = send(cSock, buffer, strlen(buffer), 0);    /* socket,  data, byte len, flags	 */

   printf("sent %d bytes\n", s);
   close(cSock);
}
