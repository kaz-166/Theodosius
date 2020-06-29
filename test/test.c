#include<stdio.h>
#include "../src/module/gpio.h"
#include "../src/common/typedef.h"
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>

static void gpioTest( void );
static void test_gpio_set( void );

void main(void)
{
	printf("Theodosius tests embarking on!\n");
	gpioTest();
	return;
}

static void gpioTest( void )
{
	CU_pSuite gpio_suite;
	
	CU_initialize_registry();
	gpio_suite = CU_add_suite("gpio_test", 0, 0);
	/*********************
	* Test about gpioToOutput()
	*********************/
	CU_add_test(gpio_suite, "GPIO SET", test_gpio_set);
	CU_console_run_tests();
	CU_cleanup_registry();

}

static void test_gpio_set( void )
{
	volatile unsigned long gpio;
	
	gpio = gpioOpen();
	
	gpioToOutput(0);
	/* GPIO0 to Low */
	gpioToLow( 0 );
	CU_ASSERT( gpioGetLevel(0) == GPIO_LOW );

	/* GPIO0 to High */
	gpioToHigh( 0 );
	CU_ASSERT( gpioGetLevel(0) == GPIO_HIGH );
}
