#include "gpio.h"

// GPIO
unsigned int volatile * const gpfsel0 = (unsigned int *) 0x20200000;

unsigned int volatile * const gpset0 = (unsigned int *) 0x2020001c;
unsigned int volatile * const gpclr0 = (unsigned int *) 0x20200028;
unsigned int volatile * const gppud = (unsigned int *) 0x20200094;
unsigned int volatile * const gppudclk0 = (unsigned int *) 0x20200098;

