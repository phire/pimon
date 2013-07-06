#include "uart.h"
#include "gpio.h"
#include "timer.h"

#define MAX_LENGTH 160

int main (void) {
	initTimer();
	initUART();

	// Setup 'ok led' as output
	gpio_function(16, GPIO_OUTPUT);

	puts("Pimon 0.1 - Debug monitor for Raspberry Pi (ARM)\r\n");
	puts("\r\n(pimon) ");
	gpio_clr(16); // turn 'ok led' on

	int length = 0;
	char buf[MAX_LENGTH];

    for(;;) { // main loop
		char c = getc();
		switch(c) {
		case '\r': // Return
			gpio_set(16); // led off
			puts("\r\n");
			buf[length] = '\0';
			puts(buf);
			puts("\r\n(pimon) ");
			length = 0;
			gpio_clr(16); // led on
			break;
		case '\b': // Backspace
			if (length > 0) {
				length--;
				puts("\b \b");
			}
			break;
		default:
			if(length < MAX_LENGTH) {
				buf[length] = c;
				length++;
				putc(c); // echo
			}
		}
    }
    return 0;
}

