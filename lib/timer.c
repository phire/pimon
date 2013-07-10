#include "timer.h"

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
