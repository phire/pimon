#include <module.h>
#include <string.h>

// Through macro and linker magic, we generate an array of function pointers...
extern initcall_t __initcalls_end[];
extern initcall_t __initcalls[];

void initModules() {
	initcall_t *fn;

	// ...which we loop through and call
	for (fn = __initcalls; fn < __initcalls_end; fn++) {
		(*fn)();
	}
}
