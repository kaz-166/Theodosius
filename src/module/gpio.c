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

/* Macro for conditional compilation */
#define ENABLE_WIRINGPI
// #undef ENABLE_WIRINGPI
#ifdef ENABLE_WIRINGPI
	#include <wiringPi.h>
#endif

#define TOTAL_NUM_OF_GPIOs 41

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPIO_LOW  0
#define GPIO_HIGH 1

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
#define GPPCR0_OFFSET  (volatile unsigned long)0xE4

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
#define GPSET(n) *((volatile unsigned long*)(gpio + GPSET0_OFFSET + 0x04*n))

/* Prototype */
static char gpio_set_to_in_out( unsigned char gpio_n, char in_or_out );
static char gpio_set_level( unsigned char gpio_n, char low_or_high );


/* Global variables */
volatile unsigned long gpio;
static unsigned char is_open = FALSE;

/****************************************************************************
 *
 *  EXTERNAL FUNCTIONS
 * 
 ****************************************************************************/
 
 /****************************************************************************
 * Initialize GPIO device driver
 * 
 * Input Parameters
 *       None
 ****************************************************************************/
volatile unsigned long gpioOpen( void )
{
	/* Mapping */    
#ifndef ENABLE_WIRINGPI
	gpio = mem_mapping( MEM_GPIO );
	is_open = TRUE;
	
	return gpio;
#else
	return wiringPiSetupGpio();
#endif
} 

/****************************************************************************
 * Set GPIO Input mode
 * 
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to input mode for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToInput( unsigned char gpio_n )
{
#ifndef ENABLE_WIRINGPI
	return gpio_set_to_in_out( gpio_n, GPIO_INPUT );
#else
	pinMode( gpio_n, INPUT );
	return 0;
#endif
}

/****************************************************************************
 * Set GPIO output mode
 * 
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to output mode for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToOutput( unsigned char gpio_n )
{
#ifndef ENABLE_WIRINGPI
	return gpio_set_to_in_out( gpio_n, GPIO_OUTPUT );
#else
	pinMode( gpio_n, OUTPUT );
	return 0;
#endif
}

/****************************************************************************
 * Set GPIO High
 * 
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to high for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToHigh( unsigned char gpio_n )
{
#ifndef ENABLE_WIRINGPI
	return gpio_set_level( gpio_n, GPIO_HIGH );
#else
	digitalWrite( gpio_n, HIGH );
	return 0;
#endif
}

/****************************************************************************
 * Set GPIO Low
 * 
 * Input Parameters
 * @gpio_n: Number of GPIO Pin
 * (If you want to set GPIO3 to low for example, please set gpio_n=3.)
 ****************************************************************************/
char gpioToLow( unsigned char gpio_n )
{
#ifndef ENABLE_WIRINGPI
	return gpio_set_level( gpio_n, GPIO_LOW );
#else
	digitalWrite( gpio_n, LOW );
	return 0;
#endif
}


/****************************************************************************
 *
 *  INTERNAL FUNCTIONS
 * 
 ****************************************************************************/
static char gpio_set_to_in_out( unsigned char gpio_n, char in_or_out )
{
	unsigned char fsel_x = 0;
	unsigned char fsel_bit = 0;
	
	/* Validation of input gpio_n */
	if( gpio_n >= TOTAL_NUM_OF_GPIOs )
	{
		printf("[FATAL] gpio_set_inOut:Too big gpio_n was given.\n");
		return -1;
	}
	
	if( is_open == FALSE )
	{
		printf("[FATAL] gpio_set_inOut:There was an access to GPIO although GPIO module was not opend.\n");
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
		printf("[FATAL] gpio_set_inOut:Invalid input params.\n");
		return -1;
	}
	return 0;
}


static char gpio_set_level( unsigned char gpio_n, char low_or_high )
{
	char reg_n;
	
	/* Validation of input gpio_n */
	if( gpio_n >= TOTAL_NUM_OF_GPIOs )
	{
		printf("[FATAL] gpio_set_level:Too big gpio_n was given.\n");
		return -1;
	}
	
	if( is_open == FALSE )
	{
		printf("[FATAL] gpio_set_level:There was an access to GPIO although GPIO module was not opend.\n");
		return -1;
	}
	
	if( low_or_high == GPIO_HIGH )
	{
		if( gpio_n < 32 )
		{
			GPSET( 0 ) = GPSET( 0 ) | (1 << gpio_n);
		}
		else
		{
			GPSET( 1 ) = GPSET( 1 ) | (1 << (gpio_n % 32 ));
		}
	}
	else if( low_or_high == GPIO_LOW )
	{
		if( gpio_n < 32 )
		{
			GPSET( 0 ) = GPSET( 0 ) & ~(1 << gpio_n);
		}
		else
		{
			GPSET( 1 ) = GPSET( 1 ) & ~(1 << (gpio_n % 32 ));
		}
	}
	else
	{
		printf("[FATAL] gpio_set_level:Invalid input params.\n");
		return -1;
	}
	return 0;
}

