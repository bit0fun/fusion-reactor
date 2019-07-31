/* setup.h file
 *
 * sets up environment for simulator
 *
 * */


#ifndef SETUPSIM_H
#define SETUPSIM_H

#include "simtypes.h"

//extern fcaddr_t insn_cnt; 		/* number of instructions for instruction space */
extern uint8_t tgt_endian;		/* Target endian format*/
extern uint8_t host_endian;		/* Host endian format*/
extern uint8_t data_align;		/* Alignment for data */

uint32_t *insn; 				/* Need to assign to instruction spot in memory */

//extern int is_diff_endian;			/* Is the endian of host and target different */


int setup_insns( uint32_t *elf_insn, uintmax_t insn_cnt);


int create_memspace( const char *filename );

#endif /* SETUPSIM_H */
