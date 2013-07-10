#include <module.h>
#include <string.h>

void addCommand(char *cmd, int (*func)(int argc, char** argv));

int test(int argc, char **argv) {
	printf("There are %d arguments\r\n", argc);
	for(int i = 0; i <= argc; i++) {
		printf("%d: '%s'\r\n", i, argv[i]);
	}
	return argc;
}

static void init() {
	addCommand("test", &test);
}

module_init(init);
