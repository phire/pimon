
// Make sure data has been written to memory
inline void clear_range(void *start, void *end) {
	__asm volatile ("mcrr p15, #0, %[end], %[start], c12" : : [end] "r" (end), [start] "r" (start) );
}

// Make sure data has been written to memory
inline void invalidate_range(void *start, void *end) {
	__asm volatile ("mcrr p15, #0, %[end], %[start], c6" : : [end] "r" (end), [start] "r" (start) );
}

// Make sure data has been written to memory and invalidate it (so it will be read back from ram)
inline void flush_range(void *start, void *end) {
	__asm volatile ("mcrr p15, #0, %[end], %[start], c14" : : [end] "r" (end), [start] "r" (start) );
}

// Invalidate a instruction cache range for self modifying code
inline void invalidate_inst_range(void *start, void *end) {
	__asm volatile ("mcrr p15, #0, %[end], %[start], c5" : : [end]"r" (end), [start]"r" (start) );
}

// Syncronise explicit memory accesses
// Non-memory opcodes will still run (this includes interrupt enable/disable)
inline void dmb() {
	__asm volatile ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) );
}

// Block until all previous explicit memory accesses have completed
// Also blocks until any previous cache managment operations have completed and write buffer is empty.
inline void dsb() {
	__asm volatile ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) );
}
