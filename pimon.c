
// GPIO
unsigned int volatile * const gpfsel1 = (unsigned int *) 0x20200004;
unsigned int volatile * const gpset0 = (unsigned int *) 0x2020001c;
unsigned int volatile * const gpclr0 = (unsigned int *) 0x20200028;

// ARM Timer
// This is a freerunning timer, at a constant 250MHz, no-matter what.
// Clocked from a seperate PLL from everything else.
unsigned int volatile * const ARM_TIMER_CTL = (unsigned int *) 0x2000b408;
unsigned int volatile * const ARM_TIMER_CNT = (unsigned int *) 0x2000b420;

void delayms(int ms) { // Busy wait using freerunning timer
	unsigned int target = *ARM_TIMER_CNT + ms * 1000;
	while(*ARM_TIMER_CNT < target);
}

int main (void) {
	// Setup 'ok led' as output
	*gpfsel1 = 1<<18 | (*gpfsel1 & ~(7<<18));

	// Setup freerunning Timer
	*ARM_TIMER_CTL = 250 << 16; // 250 divider (1mhz clock)
	*ARM_TIMER_CTL |= 1 << 9;   // enable freerunning timer

    for(;;) {
        *gpset0 = 1<<16; // off
        delayms(320);
        *gpclr0 = 1<<16; // on
        delayms(80);
    }
    return 0;
}

