MEMORY

{
	ROM (wx) : ORIGIN = 0x40000, LENGTH = 16K
	RAM (rw) : ORIGIN = 0x20000000, LENGTH = 16K

}

ENTRY(__start)

SECTIONS
{
	.text :{
		KEEP(*(.init))
		*(.text)

	} > ROM

	.data : {
		*(.data)

	} > RAM


}
