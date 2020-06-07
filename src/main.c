/* 2020/5/27 Written by Kaz-166 
 * main.c
 * 
 */
#include<stdio.h>
#include "./common/typedef.h"

void main(void)
{
	printf("Theodosius embarkings on!\n");
	gpio_init();
	gpio_get_signal( 0, 0, 0 );
	return;
}
