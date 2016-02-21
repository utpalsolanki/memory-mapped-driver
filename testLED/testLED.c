#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define AXI_BASE_ADDRESS	0x41200000

int main()
{
	int fd, i;
	void *ptr;
	unsigned val, addr, page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	//Open Memory layout as file
	fd=open("/dev/mem",O_RDWR);
	if(fd<1) {
		printf("open error\r\n");
		exit(-1);
	}

	//Pointer to physical address
	addr= AXI_BASE_ADDRESS;
	page_addr=(addr & ~(page_size-1));
	page_offset=addr-page_addr;

	//Requesting virtual address space pointer, 
	//that actually write to desired physical address.
	ptr=mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		exit(-1);
	}

	//Init GPIO as OUTPUT, 
	//this register is at offset of 1 word from base-address.
	*((unsigned *)(ptr+page_offset + 0x4)) = 0;
	while(1)
	{
		//LEDs ON
		*((unsigned *)(ptr+page_offset))= 0xff;
		sleep(1);
		//LEDs OFF
		*((unsigned *)(ptr+page_offset))= 0;
		sleep(1);
	}	
	return 0;
}
