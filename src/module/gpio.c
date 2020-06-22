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

#define TOTAL_NUM_OF_GPIOs 41

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPFSEL0_OFFSET (volatile unsigned long)0x00
#define GPFSEL1_OFFSET (volatile unsigned long)0x04
#define GPFSEL2_OFFSET (volatile unsigned long)0x08
#define GPFSEL3_OFFSET (volatile unsigned long)0x1C
#define GPFSEL4_OFFSET (volatile unsigned long)0x10
#define GPFSEL5_OFFSET (volatile unsigned long)0x14
#define GPLEV0_OFFSET  (volatile unsigned long)0x34
#define GPLEV1_OFFSET  (volatile unsigned long)0x38
#define GPSET0_OFFSET  (volatile unsigned long)0x1C
#define GPSET1_OFFSET  (volatile unsigned long)0x20
#define GPPCR0_OFFSET (volatile unsigned long)0xE4

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

#define GPFSEL(n) *((volatile unsigned long*)(gpio + 0x04*n))

/* Prototype */
static char gpioSetInOut( unsigned char gpio_n, char in_or_out );

/* Global variables */
volatile unsigned long gpio;
static unsigned char is_open = FALSE;

volatile unsigned long gpioOpen( void )
{
	/* Mapping */    
	gpio = mem_mapping( MEM_GPIO );
	is_open = TRUE;
	
	return gpio;
} 

/****************************************************************************
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to input mode for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToInput( unsigned char gpio_n )
{
	return gpioSetInOut( gpio_n, GPIO_INPUT );
}

/****************************************************************************
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to output mode for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToOutput( unsigned char gpio_n )
{
	return gpioSetInOut( gpio_n, GPIO_OUTPUT );
}



level gpioGetSignal( unsigned int reg_number, unsigned int bit_number )
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

void gpioSetSignal( unsigned int reg_number, unsigned int bit_number, level l )
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

static char gpioSetInOut( unsigned char gpio_n, char in_or_out )
{
	unsigned char fsel_x = 0;
	unsigned char fsel_bit = 0;
	
	/* Validation of input gpio_n */
	if( gpio_n >= TOTAL_NUM_OF_GPIOs )
	{
		printf("[FATAL] gpioSetInOut:Too big gpio_n was given.\n");
		return -1;
	}
	
	if( is_open == FALSE )
	{
		printf("[FATAL] gpioSetInOut:There was an access to GPIO although GPIO module was not opend.\n");
		return -1;
	}
	
	fsel_x = gpio_n / 10;
	fsel_bit = gpio_n % 10;
	
	if( in_or_out == GPIO_INPUT )	  /* Input Mode */
	{
		GPFSEL( fsel_x ) = GPFSEL( fsel_x ) | (1 << 3*fsel_bit);
	}
	else if( in_or_out == GPIO_OUTPUT ) /* Output Mode */
	{
		GPFSEL( fsel_x ) = GPFSEL( fsel_x ) & ~(1 << 3*fsel_bit);
	}
	else
	{
		printf("[FATAL] gpioSetInOut:Invalid input params.\n");
		return -1;
	}
	return 0;
}
