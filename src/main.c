/* 2020/5/27 Written by Kaz-166 
 * main.c
 * 
 */
#include<stdio.h>
#include "./common/typedef.h"
#include "./module/mem.h"
#include "./module/gpio.h"
#include <unistd.h>

void main(void)
{
	unsigned long * gpio;
	printf("Theodosius embarkings on!\n");
	// This is a sample code.
	// GPIO0: low -> 3s wait -> high -> 3s wait ->...
	gpio = gpioOpen();
	gpioToOutput( 0 );
	while( 1 )
	{
		gpioToLow( 0 );
		sleep(3);
		gpioToHigh( 0 );
		sleep(3);
	}
	
	printf("fin");
	
	return;
}
