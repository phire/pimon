SECTIONS
{
	. = 0x8000;
    .text : { *(.text*) }
	.initcalls : { 
		__initcalls = .;
		*(.initcalls*)
		__initcalls_end = . ;
	} 
    .bss : { *(.bss*) }
}
