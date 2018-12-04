#include "simtypes.h"

/* Check if host system is little or big endian */
inline int check_endian(void){
	union{
		uint32_t 	i;
		char		c[4];
	}endt = { 0xff000000 };	/* 0 if little endian, 0xff if big */

	return endt.c[0] == 0x0;	/* Make branches easier for many architectures */		
}
