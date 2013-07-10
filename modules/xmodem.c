#include <module.h>
#include <string.h>
#include <uart.h>
#include <timer.h>
#include <commands.h>

void purge() {
	unsigned char c;
	while(!getc_timeout(100, &c));
}

int receive(unsigned char *addr) {
	unsigned char block = 1;
	int retries = 0;
	int size = 0;
	
	putc(0x15); // nak
	while (retries < 20) {
		unsigned char c = 0;
		unsigned char recv_checksum;
		unsigned char checksum = 0;
		int timeout = *ARM_TIMER_CNT + 500000; // 500ms

		// soh/eot
		while(c != 0x01) {
			if(*ARM_TIMER_CNT > timeout) goto retry;
			getc_timeout(100, &c);
			if(c == 0x04) { // eot
				putc(0x06); // ack
				printf("Done, receved %d bytes.\r\n", size);
				return 0;
			}
		}

		// block number
		if(getc_timeout(500, &c)) goto retry;
		unsigned char recved_block = c;
		if(getc_timeout(500, &c)) goto retry; 
		if(255-c != recved_block) goto retry;
		if(recved_block == (block - 1)) { // previous block is being resent
			for(int i=0; i<129; i++) // discard rest of block
				if(getc_timeout(500, &c)) break; // ack even if we timeout
			putc(0x06); // ack
			continue;
		} else if(recved_block != block) { // synchronization error, abort
			purge();
			putc(0x18); // can
			printf("error: Synchronization error - aborted.\r\n");
			return -2;
		}
		for(int i=0; i< 128; i++) {
			if(getc_timeout(500, &c)) goto retry;
			checksum += c;
			*(addr + i) = c;
		}
		if(getc_timeout(500, &recv_checksum)) goto retry; // checksum

		if(checksum == recv_checksum) {
			block++;
			addr += 128;
			size += 128;
			putc(0x06); // ack
		} else {
			putc(0x15); // nak
		}
		continue;

		retry: 
			retries++;
			purge();
			putc(0x15); // nak
			continue;
	}
	printf("error: Timeout\r\n");
	return -1;
}

static int rx(int argc, char **argv) {
	if(argc != 1) {
		printf("%s: invalid arguments", argv[0]);
		return -1;
	}
	unsigned char *addr = (unsigned char *) strtol(argv[1], 0, 16);
	printf("rx at 0x%08x\r\n", addr);
	return receive(addr);
}

void xmodem_init() {
	addCommand("rx", &rx);
}

module_init(xmodem_init);
