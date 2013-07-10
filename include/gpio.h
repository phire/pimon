
#define GPIO_INPUT 0x0
#define GPIO_OUTPUT 0x1
#define GPIO_ALT0 0x4
#define GPIO_ALT1 0x5
#define GPIO_ALT2 0x6
#define GPIO_ALT3 0x7
#define GPIO_ALT4 0x3
#define GPIO_ALT5 0x2

extern unsigned int volatile * const gpfsel0;
extern unsigned int volatile * const gpset0;
extern unsigned int volatile * const gpclr0;
extern unsigned int volatile * const gppud;
extern unsigned int volatile * const gppudclk0;

inline void gpio_function(int gpio, int func) {
	*(gpfsel0 + gpio/10) = (*(gpfsel0 + gpio/10) & ~(7 << gpio%10)) |
														(func << gpio%10);
}

inline void gpio_set(int gpio) {
	*(gpset0 + gpio/32) = 1 << (gpio%32);
}

inline void gpio_clr(int gpio) {
	*(gpclr0 + gpio/32) = 1 << (gpio%32);
}
