/* 2020/5/27 Written by Kaz-166 
 * main.c
 * 
 */
#include<stdio.h>
#include "./common/typedef.h"
#include "./module/mem.h"
#include "./module/gpio.h"

void main(void)
{
	printf("Theodosius embarkings on!\n");
	gpioOpen();
	gpioToInput( 0 );
	mem_show( MEM_GPIO );
	// mem_show( MEM_AUX);
	// mem_show( MEM_EMMC );
	printf("fin");
	return;
}
