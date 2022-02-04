#include <fcntl.h>	/* for open() 				*/
#include <stdio.h>	/* for printf()				*/
#include <unistd.h>	/* for read(), write(), close() 	*/
#include <stdlib.h>	/* malloc 				*/


#define DEV_FILE 	"/dev/vma"
#define BUF_SIZE	4096


char buf[BUF_SIZE];

void dump_vmas() {
	int fd, r, i;


	printf("Dumping vma information\n");  
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

int main( int argc, char **argv ) {
	unsigned long	heapsize;
	void		*myheap;

	dump_vmas();

	heapsize = (256 << 20);	// 256-megabytes
	myheap = malloc( heapsize );
	if ( !myheap ) { 
		printf("error: malloc failed.\n");  
		exit(1); 
	}
	printf( "Allocated 0x%08lX bytes on the heap ", heapsize );
	printf( "at virtual address %08lX\n", (long unsigned int)myheap );	 

	printf( "Press <ENTER> to continue\n");
	getchar();

	dump_vmas();
	free( myheap );	
	printf( "Done. \n" );

}
