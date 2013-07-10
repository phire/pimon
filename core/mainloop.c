#include <uart.h>
#include <gpio.h>
#include <string.h>

#define MAX_LENGTH 160

extern void init();
extern int parsePrompt(char *p);

int main (void) {
	init();

	printf("Pimon 0.1 - Debug monitor for Raspberry Pi (ARM)\r\n");
	printf("\r\n(pimon) ");
	gpio_clr(16); // turn 'ok led' on

	int length = 0;
	char buf[MAX_LENGTH];

    for(;;) { // main loop
		char c = getc();
		switch(c) {
		case '\r': // Return
			gpio_set(16); // led off
			printf("\r\n");
			if(length > 0) {
				buf[length] = '\0';
				parsePrompt(buf);
			}
			printf("(pimon) ");
			length = 0;
			gpio_clr(16); // led on
			break;
		case '\b': // Backspace
			if (length > 0) {
				length--;
				printf("\b \b");
			}
			break;
		default:
			if(length < MAX_LENGTH && c >= 0x20) { // ignore non-printing chars
				buf[length] = c;
				length++;
				putc(c); // echo
			}
		}
    }
    return 0;
}

