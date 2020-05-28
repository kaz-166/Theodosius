/* 2020/5/27 Written by kaz-166
 * gpio.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/typedef.h"

#define GPIO_BASE_REG_ADDR 0x7E215000	/* GPIO Register base address */
#define GPLEV0_OFFSET (unsigned long)0x34
#define GPLEV1_OFFSET (unsigned long)0x35

#define BLOCK_SIZE 4096
#define PAGE_SIZE  4096

static volatile unsigned int* gpio_mapping( int base_addr );

/* Global variables */

volatile unsigned int* gpio;
static int mem_fd = 0;

void gpio_init( void )
{

} 

level gpio_get_signal( void )
{
	volatile unsigned int* gpio0;
	volatile unsigned int* gpio1;
	
	printf("call gpio_get_signal()\n");
	gpio = gpio_mapping(GPIO_BASE_REG_ADDR);
	gpio0 = gpio = (volatile unsigned int *)GPLEV0_OFFSET;
	gpio0 = gpio = (volatile unsigned int *)GPLEV1_OFFSET;
	printf("GPIO(0):       %d\n", gpio0);
	printf("GPIO(1):       %d\n", gpio1);

	return LOW;
}

level gpio_set_signal( level l )
{
 printf("call gpio_cet_signal()\n");
 return LOW;
}

static volatile unsigned int* gpio_mapping( int base_addr )
{
		char *gpio_mem, *gpio_map;
		
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
		
		gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1));
		if( gpio_mem == NULL )  
		{
			printf("Memory Allocation Error\n");
			exit(-1);
		}
		
		if( (unsigned long) gpio_mem % PAGE_SIZE )
		{
				gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);
		}
		
		gpio_map = (char *)mmap((caddr_t)gpio_mem,
								BLOCK_SIZE,
								PROT_READ|PROT_WRITE,
								MAP_SHARED|MAP_FIXED,
								mem_fd,
								base_addr);
		if( (signed long)gpio_map < 0 )
		{
			printf("mmap error\n");
			exit(-1);
		}
		return (volatile unsigned int*)gpio_map;
}
