#include <module.h>
#include <commands.h>
#include <string.h>
#include <mailbox.h>

extern void dump(unsigned char*, int);

unsigned int *build_buffer(unsigned int tag, unsigned int len) {
	unsigned int *buffer = (unsigned int *)0x2000;
	buffer[0] = 20 + len + 4;
	buffer[1] = 0;
	buffer[2] = tag;
	buffer[3] = len;
	buffer[4] = 0;
	int i = 5;
	while(i * 4 < (20 + len))
		buffer[i++] = 0;
	buffer[i] = 0;
	return buffer;
}

int queryTag(int argc, char **argv) {
	unsigned int tag;
	unsigned int length = 4;
	if(argc == 0) {
		printf("%s: invalid arguments\r\n", argv[0]);
		return -1;
	} else {
		tag = strtol(argv[1], 0, 16);
	}
	if(argc >= 2) length = strtol(argv[2], 0, 10);
	volatile unsigned int *buffer = build_buffer(tag, length);
	for(int i=0; i+3 <= argc; i++) {
		buffer[i+5] = strtol(argv[i+3], 0, 16);
		buffer[4] += 4;
	}
	sendTagBuffer(buffer);

	if (buffer[1] == 0x80000000 && (buffer[4] & 0x7fffffff) == 4) {
		printf("%08x\r\n", buffer[5]); // pretty printing for single int replies
	} else {
		dump((unsigned char *) buffer, buffer[0]);
	}
	return 0;
}

int sendMessage(int argc, char **argv) {
	if(argc != 1) {
		printf("%s: invalid arguments\r\n", argv[0]);
		return -1;
	}
	unsigned int message = strtol(argv[1], 0, 16);
	
	mailboxSend(message);
	printf("%08x\r\n", mailboxRecv());
	return 0;
}

void init_mailbox() {
	addCommand("tag", &queryTag);
	addCommand("mail", &sendMessage);
}

module_init(init_mailbox);
