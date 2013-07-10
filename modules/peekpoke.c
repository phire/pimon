#include <module.h>
#include <string.h>

void addCommand(char *cmd, int (*func)(int argc, char** argv));

int peek(int argc, char **argv) {
	if(argc != 1) {
		printf("peek: invalid args\r\n");
		return -1;
	}
	unsigned int *address = (unsigned int*) strtol(argv[1], 0, 16);
	printf("%08x\r\n", *address);
	return 0;
}

int poke(int argc, char **argv) {
	if(argc != 2) {
		printf("poke: invalid args\r\n");
		return -1;
	}
	unsigned int *address = (unsigned int*) strtol(argv[1], 0, 16);
	unsigned int value = strtol(argv[2], 0, 16);
	*address = value;
	return 0;
}

static void init() {
	addCommand("peek", &peek);
	addCommand("poke", &poke);
}

module_init(init);
