INPUT("boottable.o", "trace.o", "i386.o", "i8259.o", "exceptions.o", "i8253.o", "init.o", "stdlib.o", "mem.o", "keyboard.o", "floppy.o", "cmos.o", "dma.o", "tty.o", "sys.o", "process.o", "loader.o", "comms.o")

OUTPUT_FORMAT("binary")

_link_base = 0x00001000;

SECTIONS {
	.text _link_base : {
		BYTE(0xe8); LONG(_main_offset); _main_call = .;
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
