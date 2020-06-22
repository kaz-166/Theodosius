#ifndef GPIO_H
#define GPIO_H

#include "../common/typedef.h"

extern volatile unsigned long gpioOpen( void );
extern char gpioToInput( unsigned char gpio_n );
extern char gpioToOutput( unsigned char gpio_n );
extern level gpioGetSignal( unsigned int reg_number, unsigned int bit_number );
extern void gpioSetSignal( unsigned int reg_number, unsigned int bit_number, level l );
#endif
