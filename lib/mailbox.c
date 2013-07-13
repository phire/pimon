#include <memory.h>

unsigned int volatile * const mailbox_read    = (unsigned int *) 0x2000b880;
unsigned int volatile * const mailbox_write   = (unsigned int *) 0x2000b8a0;
unsigned int volatile * const mailbox_status  = (unsigned int *) 0x2000b898;

#define MAILBOX_FULL	0x80000000
#define MAILBOX_EMPTY	0x40000000

void mailboxSend(unsigned int message) {
	while((*mailbox_status & MAILBOX_FULL) != 0);

	dsb(); // make sure any cache flushes have finished

	*mailbox_write = message;
}

unsigned int mailboxRecv() {
	while((*mailbox_status & 0x40000000) != 0);

	return *mailbox_read;
}


void sendTagBuffer(volatile unsigned int *buffer) {
	/* This buffer is shared by both the arm and videocore so to keep it 
	   syncronized, we need to flush out of L1 cachebefore telling VCOS
	   about it and not read it until vcos tells us it's finished with it.

	   This buffer should never, ever share cache lines with another variable
	   a read/write to that variable might pull part of the buffer back
	   into cache. So you make sure the buffer is cache line aligned at
	   both ends.
	*/
	flush_range((void *)buffer, (void *)buffer + buffer[0]);

	// Send message to VCOS
	mailboxSend(((unsigned int)buffer & ~0xf) | 8);
	mailboxRecv(); // Block until responce
	dmb(); // Prevent memory access re-ordering
}

