#include "string.h"

int strcmp(char *a, char *b) {
	while(*a && *b && (*a == *b)) {a++; b++;}
	return *a - *b;
}
