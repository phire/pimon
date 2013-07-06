
// GPIO
unsigned int volatile * const gpfsel1 = (unsigned int *) 0x20200004;
unsigned int volatile * const gpset0 = (unsigned int *) 0x2020001c;
unsigned int volatile * const gpclr0 = (unsigned int *) 0x20200028;
unsigned int volatile * const gppud = (unsigned int *) 0x20200094;
unsigned int volatile * const gppudclk0 = (unsigned int *) 0x20200098;

// ARM Timer
// This is a freerunning timer, at a constant 250MHz, no-matter what.
// Clocked from a seperate PLL from everything else.
unsigned int volatile * const ARM_TIMER_CTL = (unsigned int *) 0x2000b408;
unsigned int volatile * const ARM_TIMER_CNT = (unsigned int *) 0x2000b420;

void initTimer() {
	// Setup freerunning Timer
	*ARM_TIMER_CTL = 250 << 16; // 250 divider (1mhz clock)
	*ARM_TIMER_CTL |= 1 << 9;   // enable freerunning timer
}

void delayms(int ms) { // Busy wait using freerunning timer
	unsigned int target = *ARM_TIMER_CNT + ms * 1000;
	while(*ARM_TIMER_CNT < target);
}

// MiniUART
unsigned char volatile * const AUX_ENABLES     = (unsigned char *) 0x20215004;
unsigned char volatile * const AUX_MU_IO_REG   = (unsigned char *) 0x20215040;
unsigned char volatile * const AUX_MU_IER_REG  = (unsigned char *) 0x20215044;
unsigned char volatile * const AUX_MU_IIR_REG  = (unsigned char *) 0x20215048;
unsigned char volatile * const AUX_MU_LCR_REG  = (unsigned char *) 0x2021504c;
unsigned char volatile * const AUX_MU_MCR_REG  = (unsigned char *) 0x20215050;
unsigned char volatile * const AUX_MU_LSR_REG  = (unsigned char *) 0x20215054;
unsigned char volatile * const AUX_MU_CNTL_REG = (unsigned char *) 0x20215060;
unsigned short volatile * const AUX_MU_BAUD_REG = (unsigned short *) 0x20215068;

void initUART() {
	*AUX_ENABLES = 1;       // enable Mini UART
	*AUX_MU_IER_REG = 0;    // disable interrupts
	*AUX_MU_CNTL_REG = 0;   // disable transmit/receive
	*AUX_MU_LCR_REG = 3;    // set 8 bit mode (8N1)
	*AUX_MU_MCR_REG = 0;    // set RTS line high
    *AUX_MU_IER_REG = 0;    // leave interrupts disabled
	*AUX_MU_IIR_REG = 0xc6; // enable and clear both tx/rx FIFOs
	*AUX_MU_BAUD_REG = ((250000000/115200)/8)-1; // 250Mhz clock, 115200 baud

	*gpfsel1 = 2<<12 | (*gpfsel1 & ~(7<<12)); // gpio14, alt5
	*gpfsel1 = 2<<15 | (*gpfsel1 & ~(7<<15)); // gpio15, alt5
	
	// remove pullups on tx/rx lines
	*gppud = 0;
	delayms(1);
	*gppudclk0 = (1 << 14) | (1 << 15);
	delayms(1);
	*gppudclk0 = 0;

	*AUX_MU_CNTL_REG = 3; // enable transmit/receive
}

void putc(char c) {
	while((*AUX_MU_LSR_REG & 0x20) == 0);
	*AUX_MU_IO_REG = c;
}

void puts(char *s) {
	while(*s != '\0') {
		putc(*s);
		s++;
	}
}

char getc() {
	while((*AUX_MU_LSR_REG & 0x1) == 0);
	return *AUX_MU_IO_REG;
}

#define MAX_LENGTH 160

int main (void) {
	initTimer();
	initUART();

	// Setup 'ok led' as output
	*gpfsel1 = 1<<18 | (*gpfsel1 & ~(7<<18));


	puts("Pimon 0.1 - Debug monitor for Raspberry Pi (ARM)\r\n");

	puts("\r\n(pimon) ");

	*gpclr0 = 1<<16; // turn 'ok led' on

	int length = 0;
	char buf[MAX_LENGTH];
    for(;;) { // main loop
		char c = getc();
		switch(c) {
		case '\r': // Return
			*gpset0 = 1<<16; // led off
			puts("\r\n");
			buf[length] = '\0';
			puts(buf);
			puts("\r\n(pimon) ");
			length = 0;
			*gpclr0 = 1<<16; // led on
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

