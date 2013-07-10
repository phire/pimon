#include <string.h>
#include <module.h>
#include <commands.h>

char *commands[20];
int (*commands_func[20])(int, char**);
int num_commands = 0;

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

static int help(int argc, char **argv) {
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

static void init(){
	addCommand("help", &help);
}

module_init(init);
