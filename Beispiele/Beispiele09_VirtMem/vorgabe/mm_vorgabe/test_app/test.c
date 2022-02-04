#include <fcntl.h>	/* for open() 				*/
#include <stdio.h>	/* for printf()				*/
#include <unistd.h>	/* for read(), write(), close() 	*/
#include <stdlib.h>	/* malloc 				*/


#define DEV_FILE 	"/dev/mm"
#define BUF_SIZE	4096


char buf[BUF_SIZE];

int main( int argc, char **argv ) {
	int fd, r, i;

	printf("Dumping mm information\n");  
	fd = open( DEV_FILE, O_RDWR );
	if ( fd < 0 ) { 
		printf("error: opening devie %s\n", DEV_FILE);  
		exit(1); 
	}

	r = read( fd, buf, BUF_SIZE );
	if (r < 0) {
		printf("error: read failed\n");
	} else {
		for (i=0; i<r; i++)
			putchar(buf[i]);
		printf("*** end ***\n");
	}

	close( fd );
}
