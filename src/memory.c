#include "memory.h"
#include <stdio.h>


/* Instruction memory */
struct mem_block insn_mem = {
		.paddr 	= 0,
		.vaddr 	= ROM_START,
		.size 	= ROM_SIZE,
		.fp		= NULL,
		.m		= NULL,
};

/* Data memory */
struct mem_block data_mem = {
		.paddr 	= 0,
		.vaddr 	= RAM_START,
		.size 	= RAM_SIZE,
		.fp		= NULL,
		.m		= NULL,
};		

uint32_t registers[32] = {0};	/* gp register file */

/* Array of pointers to memory blocks */
struct mem_block *memblocks[] = {
				&insn_mem,
				&data_mem,
			};

/* Initialize n number of memory blocks */
int init_memory( struct mem_block **mem, unsigned int n ){
	/* Allocate space for memory */
	for( unsigned int i = 0; i < n; i++ ){
		mem[i]->paddr = 0;	/* make sure beginning of translated memory is correct */
		
		/* Allocate memory for block */
		mem[i]->m = calloc( sizeof( uint8_t), mem[i]->size );

		/* Note that this may take a while due to issues with optimization
		 *
		 * Taking alignment into account, this can be changed to be much faster 
		 * */

		/* Check if fp is NULL, which then don't copy data */
		if( mem[i]->fp != NULL ){
			for( unsigned int j = 0; j < mem[i]->size; j++){
				*(mem[i]->m + j) = *(mem[i]->fp + j);	
			}

			/* No longer need file pointer, clear out value */
			mem[i]->fp = NULL;
		}
	}

	return 0;
}

/* Free up and clean out memory */
int free_memory( struct mem_block **mem, unsigned int n ){
	
	for( unsigned int i = 0; i < n; i++){
		/* Clean out memory */
		for( unsigned int j = 0; j < mem[i]->size; j++){
			*(mem[i]->m + j) = 0;
		}

		/* Deallocate block */
		free( mem[i]->m );
	}

	return 0;
}

/* Determine which block to write to, then Write n bytes to memory at 'addr' */
int write_mem( uintmax_t addr, uint8_t* data, uintmax_t n, uint8_t flag_elf_load ){
	/* Check bounds for defined cpu memory regions, special memory sections 
	 * This is independent of the running program
	 * */

	/* Specific register access, CPU paramters (?)*/
	if( addr < ROM_START ){

		/* Ignore segfault if loading from ELF */
		if( flag_elf_load ){
			_write_mem( &insn_mem, addr, data, n );
		}
		
		else{
			printf("Program attempting to write to unimplemented memory at addess: %08x\n", (unsigned int)addr);
			return SIM_SEGFAULT;
		}
	}

	/* Inside ROM/instruction memory */
	else if( addr < ( ROM_START + ROM_SIZE ) ){

		_write_mem( &insn_mem, addr, data, n );
	}

	/* Inside RAM/data memory */
	else if( ( addr >= RAM_START ) && ( addr < ( RAM_START + RAM_SIZE ))  ){
		_write_mem( &data_mem, addr, data, n );
	}
	else{
		/* Outsize of memory bounds */
		return SIM_SEGFAULT;
	}
	return 0;
}

/* Determine which block to read from, then Read n bytes from memory at 'addr' */
int read_mem( uintmax_t addr, uint8_t* data, uintmax_t n ){
	/* Check bounds for defined cpu memory regions, special memory sections 
	 * This is independent of the running program
	 * */

	/* Specific register access, CPU paramters (?)*/
	if( addr < ROM_START ){
		printf("Program attempting to read from unimplemented memory at addess: %08x\n",(unsigned int) addr);
		return SIM_SEGFAULT;
	}

	/* Inside ROM/instruction memory */
	else if( addr < ( ROM_START + ROM_SIZE ) ){
		_read_mem( &insn_mem, addr, data, n );
	}

	/* Inside RAM/data memory */
	else if( ( addr >= RAM_START ) && ( addr < ( RAM_START + RAM_SIZE ))  ){
		_read_mem( &data_mem, addr, data, n );
	}
	else{
		/* Outsize of memory bounds */
		return SIM_SEGFAULT;
	}
	return 0;
}


/* Write n bytes to memory at 'addr' */
int _write_mem( struct mem_block *mem, uintmax_t addr, uint8_t* data, uintmax_t n ){
	
	/* Check if memory is in bounds */
	if( (addr < mem->vaddr) || (addr > (mem->vaddr + mem->size)) ){
		/* Segmentation fault occurred return value */
		return SIM_SEGFAULT;
	}

	/* Translate memory, subtract virtual from address to get physical address */
	uintmax_t paddr = addr - mem->vaddr;

	for( uintmax_t i = 0; i < n; i++){
		*(mem->m + i + paddr) = *(data + i);
	}


	/* Successful write */
	return 0;
}

/* Read n bytes from memory at 'addr' */
int _read_mem( struct mem_block *mem, uintmax_t addr, uint8_t* data, uintmax_t n ){
	/* Check if memory is in bounds */
	if( (addr < mem->vaddr) || (addr > (mem->vaddr + mem->size)) ){
		/* Segmentation fault occurred return value */
		return SIM_SEGFAULT;
	}

	/* Translate memory, subtract virtual from address to get physical address */
	uintmax_t paddr = addr - mem->vaddr;

	for( uintmax_t i = 0; i < n; i++){
		 *(data + i) = *(mem->m + i + paddr);
	}

	/* Successful read */
	return 0;
}

