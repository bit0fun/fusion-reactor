/* simtypes.h
 *
 * Creates types to be used in simulator
 *
 * */

#ifndef SYMTYPE_H
#define SYMTYPE_H
#include <stdint.h>

/** type definitions **/ 
typedef uint32_t fcaddr_t; /* 32 bit address space; useful for future changes */

/** macros for converting types, data organization **/

/* useful in case 64 bit addr space, or other changes */
#define MEM2INSNPTR( MPTR ) 	(fcaddr_t *)(MPTR)	

#define ENDSWP_16( N )	( N >> 8 ) | ( N << 8 )

#define ENDSWP_32( N ) (((N & 0xff000000) >> 24) \
					  | ((N & 0x00ff0000) >> 8)  \
					  | ((N & 0x0000ff00) << 8)  \
					  |  (N << 24) )

//#define BIG_ENDIAN	0
//#define LITTLE_ENDIAN 1

uint8_t host_endian; /* Global boolean for host endianess */

inline int check_endian(void);


#endif /* SYMTYPE_H */
