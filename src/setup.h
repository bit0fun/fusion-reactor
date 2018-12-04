/* setup.h file
 *
 * sets up environment for simulator
 *
 * */


#ifndef SETUPSIM_H
#define SETUPSIM_H

#include <simtypes.h>

extern fcaddr_t insn_cnt; 		/* number of instructions for instruction space */
extern tgt_endian;				/* Target endianness*/
uint32_t insn[insn_cnt]; 		/* Need to assign to instruction spot in memory */

int setup_insns( void );



#endif /* SETUPSIM_H */
