#include<stdio.h>
#include "../src/module/gpio.h"
#include "../src/common/typedef.h"

static void gpioTest( void );

void main(void)
{
	printf("Theodosius tests embarking on!\n");
	gpioTest();
	return;
}

static void gpioTest( void )
{
	unsigned char results[64];
	unsigned char test_num = 0;
	volatile unsigned long gpio;
	
	gpio = gpioOpen();
	
	/*********************
	* Test about gpioToOutput()
	*********************/
	gpioToOutput(0);
	((*(volatile unsigned long*)(gpio) && 1 != 0)) ? (results[0] = TRUE) : (results[0] = FALSE);
	test_num++;
	
	for( int i = 0; i < test_num; i++ )
	{
		if( results[i] == TRUE )
		{
			printf("GPIO TEST[%d] SUCCESS\n", i);
		}
		else
		{
			printf("GPIO TEST[%d] FAIL\n", i);
		}
	}
}
