

typedef enum
{
	MEM_AUX,
	MEM_BSC0,
	MEM_BSC1,
	MEM_BSC2,
	MEM_EMMC,
	MEM_GPIO,
	MEM_CLK
} MemoryAddress;

extern volatile unsigned long mem_mapping( long base_addr );
extern void mem_show( MemoryAddress mem_addr );
