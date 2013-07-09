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
