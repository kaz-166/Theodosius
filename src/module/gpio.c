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

#define GPIO_BASE_REG_ADDR 0x7E200000	/* GPIO Register base address */
#define GPFSEL0_OFFSET (volatile unsigned int)0x00
#define GPLEV0_OFFSET  (volatile unsigned int)0x34
#define GPLEV1_OFFSET  (volatile unsigned int)0x38
#define GPSET0_OFFSET  (volatile unsigned int)0x1C
#define GPSET1_OFFSET  (volatile unsigned int)0x20

#define GPLEV0 *(gpio + GPLEV0_OFFSET)
#define GPLEV1 *(gpio + GPLEV1_OFFSET)
#define GPSET0 *(gpio + GPSET0_OFFSET)
#define GPSET1 *(gpio + GPSET1_OFFSET)

#define BLOCK_SIZE 4096
#define PAGE_SIZE  4096

static volatile unsigned int* gpio_mapping( int base_addr );

/* Global variables */

volatile unsigned int* gpio;
static int mem_fd = 0;

void gpio_init( void )
{
	/* Mapping */
	gpio = gpio_mapping(GPIO_BASE_REG_ADDR);
} 

level gpio_get_signal( unsigned int reg_number, unsigned int bit_number )
{
	unsigned int reg = 0x00000000;
	level ret = LOW;
	
	switch( reg_number )
	{
		/* GPLEV0 */
		case 0:
			reg = GPLEV0;
			break;
		/* GPLEV1 */
		case 1:
			reg = GPLEV1;
			break;
		/* Exception */
		default:
			perror("internal function error!!");
			break;
	}
	
	if( ( (reg) && (0x00000001 << bit_number) ) == 0 )
	{
		ret = LOW;
	}
	else
	{
		ret = HIGH;
	}

	return ret;
}

void gpio_set_signal( unsigned int reg_number, unsigned int bit_number, level l )
{
	unsigned int reg = 0x00000000;
	
	switch( reg_number )
	{
		/* GPSET0 */
		case 0:
			reg = GPSET0;
			break;
		/* GPSET1 */
		case 1:
			reg = GPSET1;
			break;
		/* Exception */
		default:
			perror("internal function error!!");
			break;
	}
	
	reg = reg | (0x00000001 << bit_number);
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
