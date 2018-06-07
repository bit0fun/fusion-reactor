#ifndef OPC_H
#define OPC_H

#include "fusion.h"
#include "fusion-opc.h"
#include "parseelf.h"

#define SEXT_14B(N)		( ((N) & 0x3FFF) | ( ( ((N) & 0x2000) ? 0xFFFFC000 : 0 ) ) )
#define SEXT_16B(N)		( ((N) & 0xFFFF) | ( ( ((N) & 0x8000) ? 0xFFFF0000 : 0 ) ) )
#define SEXT_21B(N) 	( ((N) & 0x1FFFFF) | ( ( ((N) & 0x100000) ? 0xFFE00000 : 0 ) ) )


/* Prototypes */
uint32_t add( uint32_t rsa, uint32_t rsb );
uint32_t sub( uint32_t rsa, uint32_t rsb );
uint32_t not( uint32_t rsa, uint32_t rsb ); 
uint32_t and( uint32_t rsa, uint32_t rsb ); 
uint32_t or( uint32_t rsa, uint32_t rsb );
uint32_t xor( uint32_t rsa, uint32_t rsb );
uint32_t sal(uint32_t rsa, uint32_t rsb );
uint32_t sar( uint32_t rsa, uint32_t rsb );
uint32_t sll(uint32_t rsa, uint32_t rsb );
uint32_t slr( uint32_t rsa, uint32_t rsb );
uint32_t comp( uint32_t rsa, uint32_t rsb );

uint32_t addi( uint32_t rsa, uint16_t imm ); 
uint32_t subi( uint32_t rsa, uint16_t imm ); 
uint32_t noti( uint16_t imm );
uint32_t andi( uint32_t rsa, uint16_t imm );
uint32_t ori( uint32_t rsa, uint16_t imm );
uint32_t xori( uint32_t rsa, uint16_t imm );
uint32_t sali( uint32_t rsa, uint16_t imm );
uint32_t sari( uint32_t rsa, uint16_t imm );
uint32_t slli( uint32_t rsa, uint16_t imm );
uint32_t slri( uint32_t rsa, uint16_t imm );
uint32_t compi( uint32_t rsa, uint16_t imm );

uint32_t lw( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint16_t imm );
uint32_t lth( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint16_t imm ); 
uint16_t lh( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint16_t imm );
uint8_t lb( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint16_t imm );

uint32_t li( uint16_t imm );
uint32_t lsi( uint16_t imm ); 
uint32_t lgi( uint16_t imm ); 
uint32_t lui( uint16_t imm ); 
uint32_t lusi( uint16_t imm); 
uint32_t lugi( uint16_t imm ); 
uint32_t lni( uint16_t imm );
uint32_t lnsi( uint16_t imm ); 
uint32_t lngi( uint16_t imm ); 
uint32_t luni( uint16_t imm ); 
uint32_t lunsi( uint16_t imm );
uint32_t lungi( uint16_t );

void sw( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint32_t rsb, uint16_t imm );
void sh( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint32_t rsb, uint16_t imm ); 
void sb( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint32_t rsb, uint16_t imm ); 
void sth( uint32_t** data_mem, dataseg_info dseg_i, uint32_t rsa, uint32_t rsb, uint16_t imm );
	
void j( uint32_t* pc, uint32_t imm );
uint32_t jal( uint32_t* pc,  uint32_t imm, uint32_t* ra );
void jr( uint32_t* pc, uint32_t rsa, uint32_t imm );
uint32_t jrl( uint32_t* pc, uint32_t rsa, uint32_t imm, uint32_t* ra);

int beq( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm );
int bne( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm );
int bgt( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm );
int blt( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm );


#endif /* OPC_H */
