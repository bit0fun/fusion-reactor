#ifndef REACTOR_MEM_H
#define REACTOR_MEM_H

#include <stdlib.h>
#include <stdint.h>
#include "memcfg.h"
#include "simsig.h"


struct mem_block{
	uintmax_t paddr;	/* physical address for memory block */
	uintmax_t vaddr;	/* virtual address for memory block */
	uintmax_t size;		/* size of memory block */
	uint8_t* fp;		/* Position in file to get data from */
	uint8_t* m;			/* Actual memory in block */
};

extern struct mem_block insn_mem;
extern struct mem_block data_mem;
extern struct mem_block *memblocks[];

extern uint32_t registers[32];

/* Initialize n number of memory blocks */
int init_memory( struct mem_block **mem, unsigned int n );

/* Free up and clean out memory */
int free_memory( struct mem_block **mem, unsigned int n );

/* Memory access functions */

/* Determine which block to write to, then Write n bytes to memory at 'addr' */
int write_mem( uintmax_t addr, uint8_t* data, uintmax_t n, uint8_t flag_elf_load );

/* Determine which block to read from, then Read n bytes from memory at 'addr' */
int read_mem( uintmax_t addr, uint8_t* data, uintmax_t n );


/* Internal memory access functions */

/* Write n bytes to memory at 'addr' */
int _write_mem( struct mem_block *mem, uintmax_t addr, uint8_t* data, uintmax_t n );

/* Read n bytes from memory at 'addr' */
int _read_mem( struct mem_block *mem, uintmax_t addr, uint8_t* data, uintmax_t n );

#endif
