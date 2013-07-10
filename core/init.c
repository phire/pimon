#include <uart.h>
#include <gpio.h>
#include <timer.h>
#include <string.h>
#include <module.h>

void init() {
	 // Timer is initilized first, because other init functions use delayms
	initTimer();
	initUART();
	initModules();

	// Setup 'ok led' as output
	gpio_function(16, GPIO_OUTPUT);
}
