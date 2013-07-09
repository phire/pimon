int strcmp(char *a, char *b);
long int strtol(char *nptr, char **endptr, int base);
char * itoa(int value, char *str, int base);
char * utoa(unsigned int value, char *str, int base);
int strlen(char *s);

typedef __builtin_va_list va_list;
int printf(char *format, ...);
int vprintf(char *fmt, va_list args);
