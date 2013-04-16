#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#define MEM_BASE_ADDR	 0x09000000



int main(int argc, char ** argv){
	int fd ;
	int i, page_size ;
	char ** endptr ;
	unsigned int offset, val ;
	unsigned short fifo_state, fifo_data ;
	volatile unsigned short * gpmc_pointer ; 

	unsigned short dummy_data [10] ;
	unsigned short ledVal = 0x00AA;
	if(argc < 2){
		printf("Usage : write_mem <offset> <val> \n ex : write_mem 400 0x55AA \n");	
		return -1 ;
	}
	offset = strtoul(argv[1], endptr,0 );
	val = strtoul(argv[2], endptr ,0);
	printf("Writing 0x%04x at %d \n", val, offset);

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd == -1){
		perror("Error opening file");
		printf("error opening /dev/mem \n");
		exit(EXIT_FAILURE);
	}


	page_size = getpagesize();

	gpmc_pointer = (volatile unsigned short *) mmap(0, 8*page_size, 
	PROT_READ | 
	PROT_WRITE, 
	MAP_SHARED ,fd, 
	MEM_BASE_ADDR);
	if(gpmc_pointer == -1){
		printf("cannot allocate pointer on %x \n", MEM_BASE_ADDR);
		exit(EXIT_FAILURE);
	}
	gpmc_pointer[offset/2] = val ;
	munmap((void *) gpmc_pointer, page_size);
	close(fd);
	return 0 ;
}