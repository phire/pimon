#include "uart.h"
#include "string.h"

// gcc's builtin implementation of va_arg
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
typedef __builtin_va_list va_list;

void puts(char *);

int printf(char *format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vprintf(format, args);
	va_end(args);
	return ret;
}

int vprintf(char *fmt, va_list args) {
	int count = 0;
	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			char *str = 0;
			int zero_pad = 0;
			int space_pad = 0;
			char buf[17];
			while(!str) {
				switch(*fmt++) {
				case '0':
					zero_pad = strtol(fmt, &fmt, 10); 
					break;
				case '-':
					space_pad = strtol(fmt, &fmt, 10);
					break;
				case 'x':
				case 'X':
					utoa(va_arg(args, unsigned int), buf, 16);
					str = buf;
					break;
				case 'd':
					itoa(va_arg(args, int), buf, 10);
					str = buf;
					break;
				case 's':
					str = va_arg(args, char *);
					break;
				}
			}
			int len = strlen(str);
			int padding = 0;
			char pad_char = '\0';
			if(zero_pad && space_pad == 0) {
				pad_char = '0';
				padding = zero_pad - len;
			} else if (space_pad) {
				pad_char = ' ';
				padding = space_pad - len;
			}
			count += padding + len;
			while(padding-- > 0) putc(pad_char);
			puts(str);
		} else { 
			putc(*fmt++);
			count ++;
		}
	}
	return count;
}
