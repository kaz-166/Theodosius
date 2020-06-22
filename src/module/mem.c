#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/typedef.h"
#include "./mem.h"

#define BLOCK_SIZE 4096
#define PAGE_SIZE  4096

#define BASE_ADDR (unsigned long)0x20000000	/* GPIO Register base address */
#define GPIO_BASE_REG_ADDR (unsigned long)0x00200000	/* GPIO Register base address */
#define SERIAL_BASE_REG_ADDR (unsigned long)0x00201000

volatile unsigned long mem_mapping( long base_addr )
{
	char *mem;
	unsigned long map;
	int mem_fd = 0;
	
	if( !mem_fd )
	{
		mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
		if( mem_fd < 0 )
		{
			printf("Can't Opem /dev/mem \n");
			printf("ERRNO: %d\n", errno);
			exit(-1);
		}
	}
	
	mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1));
	if( mem == NULL )  
	{
		printf("Memory Allocation Error\n");
		exit(-1);
	}
	
	if( (unsigned long) mem % PAGE_SIZE )
	{
		mem += PAGE_SIZE - ((unsigned long)mem % PAGE_SIZE);
	}
	printf("mmap: base_addr   %x \n", base_addr);
	
	map = (unsigned long)mmap((caddr_t)mem,
							BLOCK_SIZE,
							PROT_READ|PROT_WRITE,
							MAP_SHARED|MAP_FIXED,
							mem_fd,
							base_addr);
	printf("mmap: allocated   %x \n", map);

	return (volatile unsigned long)map;
}

void mem_show( MemoryAddress mem_addr )
{
	unsigned long addr;
	volatile unsigned long map;
	
	switch( mem_addr )
	{
		case MEM_GPIO:
			addr = BASE_ADDR + GPIO_BASE_REG_ADDR;
			printf("=================== GPIO ==================\n");
			break;
		case MEM_SERIAL:
			addr = BASE_ADDR + SERIAL_BASE_REG_ADDR;
			printf("================== SERIAL =================\n");
			break;
		default:
			break;
	}
	
	map = mem_mapping( addr );
	
	for(int i = 0; i < 256; i++)
	{
		printf("%x:  %x\n", addr+i, *((unsigned int*)(map + i)));
	}
	
	munmap( (unsigned long *)map, BLOCK_SIZE );
}
