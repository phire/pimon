#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "string.h"

char *commands[20];
int (*commands_func[20])(int, char**);
int num_commands = 0;

int help(int argc, char **argv) {
	puts("Supported commands: ");
	for(int i = 0; i < num_commands; i++) {
		puts(commands[i]); 
		if(i != num_commands - 1)
			puts(", ");
		else
			puts("\r\n");
	}
	return 0;
}

int test(int argc, char **argv) {
	puts("There are "); putc('0' + argc); puts(" arguments\r\n");
	for(int i = 0; i <= argc; i++) {
		putc('0' + i); puts(": '"); puts(argv[i]); puts("'\r\n");
	}
	return argc;
}

int peek(int argc, char **argv) {
	if(argc != 1) {
		puts("peek: invalid args\r\n");
		return -1;
	}
	unsigned int *address = (unsigned int*) strtol(argv[1], 0, 16);
	puthex(*address);
	puts("\r\n");
	return 0;
}

int poke(int argc, char **argv) {
	if(argc != 2) {
		puts("poke: invalid args\r\n");
		return -1;
	}
	unsigned int *address = (unsigned int*) strtol(argv[1], 0, 16);
	unsigned int value = strtol(argv[2], 0, 16);
	*address = value;
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
	puts("pimon: "); puts(argv[0]);	puts(": command not found\r\n");
	return -1;
}

#define MAX_LENGTH 160

int main (void) {
	initTimer();
	initUART();

	num_commands = 0; // BSS isn't being zeroed yet
	addCommand("help", &help);
	addCommand("test", &test);
	addCommand("peek", &peek);
	addCommand("poke", &poke);

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
			if(length > 0) {
				buf[length] = '\0';
				parsePrompt(buf);
			}
			puts("(pimon) ");
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
			if(length < MAX_LENGTH && c >= 0x20) { // ignore non-printing chars
				buf[length] = c;
				length++;
				putc(c); // echo
			}
		}
    }
    return 0;
}

