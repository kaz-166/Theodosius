

typedef enum
{
	MEM_GPIO,
	MEM_SERIAL
} MemoryAddress;

extern volatile unsigned long mem_mapping( long base_addr );
extern void mem_show( MemoryAddress mem_addr );
