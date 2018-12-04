#include "setup.h"

int setup_insns( uint32_t *elf_insn ){
	if( tgt_endian != host_endian ) {	/* Only matters if there is a problem */
		/* Fix instructions for execution to be opposite endian */		
		int tmp_insn = 0;	/* Temporary value for instruction */
		for( int i = 0; i < insn_cnt; i++){
			insn[i] = ENDSWP_32( elf_insn[i] ); 	
		}
	
	} else {
		for( int i = 0; i < insn_cnt; i++){
			insn[i] = elf_insn[i];
		}
	}

	return 0;
}
