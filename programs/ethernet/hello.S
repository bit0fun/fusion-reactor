.globl __start


.data 
	VAR:	.word 0xcafe
	VAR2:	.half 0xff

.text

__start:
	nop
	nop

main:
	li  gr2, 0 
	li  gr3, 16 
	li  gr5, 0x68
	li  tmp0, 16

loop:
	sw		gr5, 0x040028(zero)
	subi	tmp0, tmp0, 1
	addi 	gr5, gr5, 3
	bne	tmp0, zero, loop 
done:
	
	
