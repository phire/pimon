#include "string.h"

int strcmp(char *a, char *b) {
	while(*a && *b && (*a == *b)) {a++; b++;}
	return *a - *b;
}

long int strtol(char *nptr, char **endptr, int base) {
	// Doesn't currently support, whitespace, +/- or 0x prefixes
	long int ret = 0;
	char *p = nptr;
	int valid = 0;
	for(char c = *p;; c = *(++p)) {
		int val;
		if(c < '0') break;
		else if(c <= '9') val = c - '0';
		else if((c | 0x20) < 'a' || (c | 0x20) > 'z') break;
		else val = (c | 0x20) - ('a' - 10);
		if(val > base) break;
		valid = 1;
		ret = ret * base + val;
	}
	if(endptr) *endptr = valid ? p : nptr;
	return ret;
}

char * itoa(int value, char *str, int base) {
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 ) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 ) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr ) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

char * utoa(unsigned int value, char *str, int base) {
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 ) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Use the same string as itoa, this allows gcc to fold them both together and uses less space
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr ) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

int strlen(char *s) {
	int count = 0;
	while(*s++ != '\0') count++;
	return count;
}
