#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "string.h"
#include <module.h>

char *commands[20];
int (*commands_func[20])(int, char**);
int num_commands = 0;

int help(int argc, char **argv) {
	printf("Supported commands: ");
	for(int i = 0; i < num_commands; i++) {
		printf(commands[i]); 
		if(i != num_commands - 1)
			printf(", ");
		else
			printf("\r\n");
	}
	return 0;
}

void addCommand(char *cmd, int (*func)(int argc, char** argv)) {
	commands[num_commands] = cmd;
	commands_func[num_commands] = func;
	num_commands++;
}

int parsePrompt(char *p) {
	int argc = 0;
	char *argv[9]; // Max of 9 arguments
	argv[0] = p;
	while(*p != '\0' && argc < 9) {
		if(*p == ' ') {
			*p = '\0'; // replace spaces with null chars
			argv[++argc] = ++p;
		} else {
			p++;
		}
	}
	for(int i=0; i < num_commands; i++) {
		if(strcmp(argv[0], commands[i]) == 0)
			return commands_func[i](argc, argv);
	}
	printf("pimon: %s: command not found\r\n", argv[0]);
	return -1;
}

#define MAX_LENGTH 160

int main (void) {
	initTimer();
	initUART();
	initModules();

	addCommand("help", &help);

	// Setup 'ok led' as output
	gpio_function(16, GPIO_OUTPUT);

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

