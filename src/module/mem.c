#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "../common/typedef.h"
#include "./mem.h"

#define BLOCK_SIZE 4096
#define PAGE_SIZE  4096

#define BASE_ADDR (unsigned long)0xFE000000
/* Auxilliaries(UART, SPI1 and SPI2) Registers */
#define AUX_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00215000
#define AUX_START_REG_ADDR AUX_BASE_REG_ADDR
#define AUX_END_REG_ADDR   AUX_START_REG_ADDR + 0xD4
/* BSC(Broadcom Serial Controller, I2C) Registers */
#define BSC0_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00205000
#define BSC0_START_REG_ADDR BSC0_BASE_REG_ADDR
#define BSC0_END_REG_ADDR   BSC0_START_REG_ADDR + 0x1C
#define BSC1_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00804000
#define BSC1_START_REG_ADDR BSC1_BASE_REG_ADDR
#define BSC1_END_REG_ADDR   BSC1_START_REG_ADDR + 0x1C
#define BSC2_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00805000
#define BSC2_START_REG_ADDR BSC2_BASE_REG_ADDR
#define BSC2_END_REG_ADDR   BSC2_START_REG_ADDR + 0x1C
/* EMMC(External Mass Media Controller) Registers */
#define EMMC_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00300000
#define EMMC_START_REG_ADDR EMMC_BASE_REG_ADDR
#define EMMC_END_REG_ADDR   EMMC_START_REG_ADDR + 0xFC
/* GPIO Resisters */
#define GPIO_BASE_REG_ADDR  BASE_ADDR + (unsigned long)0x00200000
#define GPIO_START_REG_ADDR GPIO_BASE_REG_ADDR
#define GPIO_END_REG_ADDR   GPIO_START_REG_ADDR + 0xB0
/* Clock Magnager Registers */
#define CLK_BASE_REG_ADDR  (unsigned long)0x00101000
#define CLK_START_REG_ADDR (unsigned long)0x00101070
#define CLK_END_REG_ADDR   (unsigned long)0x00101084

volatile unsigned long mem_mapping( MemoryAddress mem_addr )
{
	char *mem;
	unsigned long map;
	int mem_fd = 0;
	long base_addr;
	
	switch( mem_addr )
	{
		case MEM_AUX:
			base_addr = AUX_BASE_REG_ADDR;
			break;
		case MEM_BSC0:
			base_addr = BSC0_BASE_REG_ADDR;
			break;
		case MEM_BSC1:
			base_addr = BSC1_BASE_REG_ADDR;
			break;
		case MEM_BSC2:
			base_addr = BSC2_BASE_REG_ADDR;
			break;
		case MEM_EMMC:
			base_addr = EMMC_BASE_REG_ADDR;
			break;
		case MEM_GPIO:
			base_addr = GPIO_BASE_REG_ADDR;
			break;
		case MEM_CLK:
			base_addr = CLK_BASE_REG_ADDR;
			break;
		default:
			printf("[FATAL] mem_mapping:Invalid MemoryAddress\n");
			exit(-1);
			break;
	}
	
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
	
	mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1));
	if( mem == NULL )  
	{
		printf("Memory Allocation Error\n");
		exit(-1);
	}
	
	if( (unsigned long) mem % PAGE_SIZE )
	{
		mem += PAGE_SIZE - ((unsigned long)mem % PAGE_SIZE);
	}
	
	map = (unsigned long)mmap((caddr_t)mem,
							BLOCK_SIZE,
							PROT_READ|PROT_WRITE|PROT_EXEC,
							MAP_SHARED|MAP_LOCKED,
							mem_fd,
							base_addr);

	return (volatile unsigned long)map;
}

void mem_show( MemoryAddress mem_addr )
{
	unsigned long start_addr;
	unsigned long end_addr;
	volatile unsigned long map;
	
	switch( mem_addr )
	{
		case MEM_AUX:
			start_addr = AUX_START_REG_ADDR;
			end_addr   = AUX_END_REG_ADDR;
			printf("=================== AUX ===================\n");
			break;
		case MEM_BSC0:
			start_addr = BSC0_START_REG_ADDR;
			end_addr   = BSC0_END_REG_ADDR;
			printf("=================== BSC0 ===================\n");
			break;
		case MEM_BSC1:
			start_addr = BSC1_START_REG_ADDR;
			end_addr   = BSC1_END_REG_ADDR;
			printf("=================== BSC1 ===================\n");
			break;
		case MEM_BSC2:
			start_addr = BSC2_START_REG_ADDR;
			end_addr   = BSC2_END_REG_ADDR;
			printf("=================== BSC2 ===================\n");
			break;
		case MEM_EMMC:
			start_addr = EMMC_START_REG_ADDR;
			end_addr   = EMMC_END_REG_ADDR;
			printf("=================== EMMC ==================\n");
			break;
		case MEM_GPIO:
			start_addr = GPIO_START_REG_ADDR;
			end_addr   = GPIO_END_REG_ADDR;
			printf("=================== GPIO ==================\n");
			break;
		case MEM_CLK:
			start_addr = CLK_START_REG_ADDR;
			end_addr   = CLK_END_REG_ADDR;
			printf("=================== CLKMAN ================\n");
			break;
		default:
			break;
	}
	
	map = mem_mapping( mem_addr );
	
	for(int i = 0; i <= (end_addr-start_addr); i=i+4)
	{
		printf("%x:  %x\n", start_addr+i, *((unsigned int*)(map + i)));
	}
	
	munmap( (unsigned long *)map, BLOCK_SIZE );
}
