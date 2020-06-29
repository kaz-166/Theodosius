#ifndef GPIO_H
#define GPIO_H

#include "../common/typedef.h"

#define GPIO_LOW  0
#define GPIO_HIGH 1

extern volatile unsigned long gpioOpen( void );
extern char gpioClose( void );
extern char gpioToInput( unsigned char gpio_n );
extern char gpioToOutput( unsigned char gpio_n );
extern char gpioToHigh( unsigned char gpio_n );
extern char gpioToLow( unsigned char gpio_n );
extern char gpioGetLevel( unsigned char gpio_n );
#endif
