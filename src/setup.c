#include "setup.h"
#include "../fusion/fusion-elf.h"

is_diff_endian = tgt_endian != host_endian

int setup_insns( uint32_t *elf_insn ){
	if( is_diff_endian == 0 ) {	
		for( int i = 0; i < insn_cnt; i++){
			insn[i] = elf_insn[i];
		}
	
	} else { /* Only matters if there is a difference */
		/* Fix instructions for execution to be opposite endian */		
		int tmp_insn = 0;	/* Temporary value for instruction */
		for( int i = 0; i < insn_cnt; i++){
			insn[i] = ENDSWP_32( elf_insn[i] ); 	
		}

	}

	return 0;
}

/* Fix the data segment to the correct endian
 * Data needs to be aligned at 4 or 16 bytes, 
 * to ensure that this change will work. Otherwise,
 * errors will occur in the change */
int fix_data_align16( uint16_t *elf_data, uint32_t data_size ){
	for(uint32_t i; i < data_size; i++){
		data[i] = ENDSWP_16(elf_data[i]);
	}
	return 0;
}

int fix_data_align32( uint32_t *elf_data, uint32_t data_size ){
	for(uint32_t i; i < data_size; i++){
		data[i] = ENDSWP_32(elf_data[i]);
	}
	return 0;
}
