#include "uart.h"
#include "gpio.h"
#include "timer.h"

// MiniUART
unsigned int volatile * const AUX_ENABLES     = (unsigned int *) 0x20215004;
unsigned int volatile * const AUX_MU_IO_REG   = (unsigned int *) 0x20215040;
unsigned int volatile * const AUX_MU_IER_REG  = (unsigned int *) 0x20215044;
unsigned int volatile * const AUX_MU_IIR_REG  = (unsigned int *) 0x20215048;
unsigned int volatile * const AUX_MU_LCR_REG  = (unsigned int *) 0x2021504c;
unsigned int volatile * const AUX_MU_MCR_REG  = (unsigned int *) 0x20215050;
unsigned int volatile * const AUX_MU_LSR_REG  = (unsigned int *) 0x20215054;
unsigned int volatile * const AUX_MU_CNTL_REG = (unsigned int *) 0x20215060;
unsigned int volatile * const AUX_MU_BAUD_REG = (unsigned int *) 0x20215068;

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

void puthex(unsigned int x) {
	static const char *hex = "0123456789abcdef";
	for(int i=0; i<8; i++, x >>= 4)
		putc(hex[x&0xf]);
}

char getc() {
	while((*AUX_MU_LSR_REG & 0x1) == 0);
	return *AUX_MU_IO_REG;
}

// returns 1 if a timeout occurs
int getc_timeout(int ms, unsigned char *c) {
	int timeout = *ARM_TIMER_CNT + ms * 1000;
	while ((*AUX_MU_LSR_REG & 0x1) == 0) 
		if(*ARM_TIMER_CNT > timeout) return 1;
	*c = *AUX_MU_IO_REG;
	return 0;
}
