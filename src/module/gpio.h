#include "../common/typedef.h"

extern void gpio_init( void );
level gpio_get_signal( unsigned int reg_number, unsigned int bit_number );
void gpio_set_signal( unsigned int reg_number, unsigned int bit_number, level l );
