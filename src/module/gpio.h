#ifndef GPIO_H
#define GPIO_H

#include "../common/typedef.h"

extern volatile unsigned long gpioOpen( void );
extern char gpioToInput( unsigned char gpio_n );
extern char gpioToOutput( unsigned char gpio_n );
extern char gpioToHigh( unsigned char gpio_n );
extern char gpioToLow( unsigned char gpio_n );
#endif
