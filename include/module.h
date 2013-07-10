typedef void (*initcall_t)();
#define module_init(x) static initcall_t __initcall_##x \
						__attribute__ ((used,section(".initcalls"))) = x;

void initModules();
