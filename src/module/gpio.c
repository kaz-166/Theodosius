/* 2020/5/27 Written b]
 * written by kaz-166
 * gpio.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/typedef.h"
#include "./mem.h"


#define GPFSEL0_OFFSET (volatile unsigned long)0
#define GPFSEL1_OFFSET (volatile unsigned long)1
#define GPFSEL2_OFFSET (volatile unsigned long)2
#define GPFSEL3_OFFSET (volatile unsigned long)3
#define GPFSEL4_OFFSET (volatile unsigned long)4
#define GPFSEL5_OFFSET (volatile unsigned long)5
#define GPLEV0_OFFSET  (volatile unsigned long)0x0034
#define GPLEV1_OFFSET  (volatile unsigned long)0x0038
#define GPSET0_OFFSET  (volatile unsigned long)0x1c
#define GPSET1_OFFSET  (volatile unsigned long)0x20
#define GPPCR0_OFFSET (volatile unsigned long)0x00E4

#define GPFSEL0 *((volatile unsigned long*)(gpio + GPFSEL0_OFFSET))
#define GPFSEL1 *((volatile unsigned long*)(gpio + GPFSEL1_OFFSET))
#define GPFSEL2 *((volatile unsigned long*)(gpio + GPFSEL2_OFFSET))
#define GPFSEL3 *((volatile unsigned long*)(gpio + GPFSEL3_OFFSET))
#define GPFSEL4 *((volatile unsigned long*)(gpio + GPFSEL4_OFFSET))
#define GPFSEL5 *((volatile unsigned long*)(gpio + GPFSEL5_OFFSET))
#define GPLEV0  *((volatile unsigned long*)(gpio + GPLEV0_OFFSET))
#define GPLEV1  *((volatile unsigned long*)(gpio + GPLEV1_OFFSET))
#define GPSET0  *((volatile unsigned long*)(gpio + GPSET0_OFFSET))
#define GPSET1  *((volatile unsigned long*)(gpio + GPSET1_OFFSET))
#define GPPCR0  *((volatile unsigned long*)(gpio + GPPCR0_OFFSET))

/* Global variables */
volatile unsigned long gpio;

void gpio_open( void )
{
	/* Mapping */    
	gpio = mem_mapping( MEM_GPIO );
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
