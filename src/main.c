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
	printf("Theodosius embarkings on!\n");
	// This is a sample code.
	gpioOpen();
	gpioToOutput( 3 );
	printf("fin");
	return;
}
