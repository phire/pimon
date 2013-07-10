// UART driver

// Initiliaze uart driver
void initUART();

// send a single charater
void putc(char c);

// send a null terminated string
//void puts(char *s);

// Send a hex string
void puthex(unsigned int x);

// get a single charater
char getc();
int getc_timeout(int ms, unsigned char *c);
