INPUT("proc4.o", "crt.o", "ipc.o")

OUTPUT_FORMAT("binary")

_link_base = 0x20000000;

SECTIONS {
	.text _link_base : {
		BYTE(0xe8); LONG(_main_offset); _main_call = .;
		BYTE(0x43); BYTE(0x53); BYTE(0x33);
		. = ALIGN(4);
		LONG(_link_base);
		LONG(_heap_base);
		. = ALIGN(4);
		*(.text)
	}
	. = ALIGN(4);
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
	.rodata : {
		*(.rodata)
	}

	_heap_base = .;
	_main_offset = main - _main_call;

	/DISCARD/ : {
		*(.*)
	}
}
