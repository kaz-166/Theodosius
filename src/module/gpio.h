#include "../common/typedef.h"

extern void gpio_open( void );
extern level gpio_get_signal( unsigned int reg_number, unsigned int bit_number );
extern void gpio_set_signal( unsigned int reg_number, unsigned int bit_number, level l );
