#include <fcntl.h>	/* for open() 				        */
#include <stdio.h>	/* for printf()				        */
#include <unistd.h>	/* for read(), write(), close() 	*/
#include <string.h>  


#define BUFF_SIZE   256


char buf[BUFF_SIZE];

int main( int argc, char **argv ) {
	int fd;
    int read_bytes, written_bytes;

	if ( argc < 4 ) {
		printf("test: usage 'test dev_file' w <string> | r <bytes> \n");
		return -1;
	}

	fd = open( argv[1], O_RDWR );
	if ( fd < 0 ) { 
		printf("test: error opening file\n");  
		return -1; 
	}
	printf("test: opened '%s' successfully.\n", argv[1]);  

    if (argv[2][0]=='w') {
   	   written_bytes = write( fd, argv[3], strlen(argv[3]) );
	   printf("test: number of bytes written = %d, of string = '%s' successfully.\n", written_bytes, argv[3]);  
    } else if (argv[2][0]=='r') {
	   read_bytes = read( fd, buf, BUFF_SIZE-1 );
       buf[read_bytes] = '\0';
	   printf("test: number of bytes read = %d, string = '%s' successfully.\n", read_bytes, buf);  
    } else {
	   printf("test: usage 'test dev_file' w <string> | r <bytes> \n");
       close( fd );
	   return -1;
    }

	close( fd );
}
