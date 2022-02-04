#include <fcntl.h>	/* for open() 				*/
#include <stdio.h>	/* for printf()				*/
#include <unistd.h>	/* for read(), write(), close() 	*/

#define BUFF_SIZE   256


char buf[BUFF_SIZE];

int main( int argc, char **argv ) {
	int fd;
    int read_bytes;

	if ( argc == 1 ) {
		printf("test: usage 'test dev_file'\n");
		return -1;
	}

	fd = open( argv[1], O_RDWR );
	if ( fd < 0 ) { 
		printf("test: error opening file\n");  
		return -1; 
	}
	printf("test: opened %s successfully.\n", argv[1]);  


	read_bytes = read( fd, buf, BUFF_SIZE-1 );
    if (read_bytes < 0)
		printf("test: read failed\n");

	printf("test: read %d bytes successfully.\n", read_bytes);  

	close( fd );
}
