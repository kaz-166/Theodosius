/* 2020/5/27 Written by Kaz-166 
 * main.c
 * 
 */
#include<stdio.h>
#include "./common/typedef.h"
#include "./module/mem.h"

void main(void)
{
	printf("Theodosius embarkings on!\n");
	//gpio_open();
	mem_show( MEM_GPIO );
	mem_show( MEM_AUX);
	mem_show( MEM_EMMC );
	printf("fin");
	return;
}
