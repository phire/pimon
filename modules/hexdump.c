#include <module.h>
#include <string.h>
#include <commands.h>

int hexdump(int argc, char **argv) {
	if (argc == 0 || argc > 2) {
		printf("%s: invalid arguments", argv[0]);
		return -1;
	}
	unsigned char *addr = (unsigned char *) strtol(argv[1], 0, 16);
	int length = 0x100;
	if(argc == 2) length = strtol(argv[2], 0, 16);

	unsigned char *ptr = (unsigned char *)((int)addr & ~0xf);// 16 byte alignment
	while ( ptr < addr + length ) {
		printf("%08x: ", ptr); // address label at start of line
		
		unsigned char buf[17];
		for(int i = 0; i < 16; ptr++, i++) {
			// we align the hex dump to 16 byte boundries, but replace anything
			// out of the requested range with padding.
			if(ptr < addr || ptr > (addr + length)) {
				buf[i] = ' ';
				printf("  ");
			} else {
				buf[i] = *ptr; // only read once, we might be dumping registers
				printf("%02x", buf[i]);
			}
			if(i & 1) printf(" "); // put a space between every second byte

			// replace non-printing chars with a dot
			if(buf[i] < 0x20 || buf[i] > 0x7e) buf[i] = '.'; 
		}
		buf[16] = '\0'; // null terminate
		printf(" %s\r\n", buf); // print ascii dump at end of line
	}

	return 0;
}

void init_hexdump() {
	addCommand("dump", &hexdump);
}

module_init(init_hexdump);
