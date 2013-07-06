#include "uart.h"
#include "gpio.h"
#include "timer.h"

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

	gpio_function(14, GPIO_ALT5);
	gpio_function(15, GPIO_ALT5);
	
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

