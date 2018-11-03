#include "opc.h"

/* Integer */
uint32_t add( uint32_t rsa, uint32_t rsb ){ 
  return rsa + rsb;
}

uint32_t sub( uint32_t rsa, uint32_t rsb ){
	return rsa - rsb;
}

uint32_t not( uint32_t rsa, uint32_t rsb ){ 
	return ( rsb ) ? ( ~(rsa) + 1 ) : ( ~(rsa) );
}

uint32_t and( uint32_t rsa, uint32_t rsb ){ 
	return rsa & rsb;
}

uint32_t or( uint32_t rsa, uint32_t rsb ){ 
	return rsa | rsb;
}

uint32_t xor( uint32_t rsa, uint32_t rsb ){
	return rsa ^ rsb;
}

uint32_t sal(uint32_t rsa, uint32_t rsb ){
	return  rsa << rsb;
}

uint32_t sar( uint32_t rsa, uint32_t rsb ){
	/* Can't use normal left shift, for unsigned numbers output can be
	 * aritmetic or logical */
	return rsa / (2 ^ (rsb) );
}

uint32_t sll(uint32_t rsa, uint32_t rsb ){
	return rsa << rsb;
}

uint32_t slr( uint32_t rsa, uint32_t rsb ){
	return rsa >> rsb;
}

uint32_t comp( uint32_t rsa, uint32_t rsb ){
	return ( rsa > rsb ) ? (uint32_t)1 : (( rsa < rsb ) ? (uint32_t)(0xFFFFFFFE) : 0 );
}


/* Immediate */
uint32_t addi( uint32_t rsa, uint16_t imm ){ 
	return rsa + (uint32_t)( imm & 0x0fff );
}

uint32_t subi( uint32_t rsa, uint16_t imm ){ 
	return rsa - (uint32_t)( imm & 0x0fff );
}
 
uint32_t noti( uint16_t imm ){ 
	return (uint32_t)(~( imm & 0x0fff));
}

uint32_t andi( uint32_t rsa, uint16_t imm ){ 
	return rsa & (uint32_t)( imm & 0x0fff );
}

uint32_t ori( uint32_t rsa, uint16_t imm ){ 
	return rsa | (uint32_t)( imm & 0x0fff );
}

uint32_t xori( uint32_t rsa, uint16_t imm ){
	return rsa ^ (uint32_t)( imm & 0x0fff );
}

uint32_t sali( uint32_t rsa, uint16_t imm ){
	return rsa << (uint32_t)( imm & 0x0fff );
}

uint32_t sari( uint32_t rsa, uint16_t imm ){
	return  rsa / (  2^((uint32_t)(imm & 0xfff) ));
}
uint32_t slli( uint32_t rsa, uint16_t imm ){
	return rsa << (uint32_t)( imm & 0x0fff );
}

uint32_t slri( uint32_t rsa, uint16_t imm ){
	return rsa >> (uint32_t)( imm & 0x0fff );
}

uint32_t compi( uint32_t rsa, uint16_t imm ){
	return ( rsa > imm ) ? (uint32_t)1 : (( rsa < imm ) ? (uint32_t)(0xFFFFFFFE) : 0 );
}


/* Load */
uint32_t lw (uint32_t rsa, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;
	union ptr2uint d_addr; /* address to access from */
	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory read detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsa);
		return 0; /* return 0 for segfault or other memory error */
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) ) ;

		d_addr.uint = dmem_p.uint + offset;
		uint32_t tmp_value = *((uint32_t *) d_addr.ptr); /* read from memory*/

		return tmp_value;

	}
}

uint32_t lth( uint32_t rsa, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;
	union ptr2uint d_addr; /* address to access from */
	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory read detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsa);
		return 0; /* return 0 for segfault or other memory error */
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint);
		
		d_addr.uint = dmem_p.uint + offset;
		uint32_t tmp_value = *((uint32_t *) d_addr.ptr); /* read from memory*/

		return (tmp_value & 0xFFFFFF00) >> 8; /* May need to double check, due to running big endian on little endian */
	}
}

uint16_t lh( uint32_t rsa, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;
	union ptr2uint d_addr; /* address to access from */
	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory read detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsa);
		return 0; /* return 0 for segfault or other memory error */
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint) >> 2;
		d_addr.uint = dmem_p.uint + offset;
		uint32_t tmp_value = *((uint32_t *) d_addr.ptr); /* read from memory*/
		return (uint16_t)(( tmp_value & 0xFFFF0000) >> 16); /* May need to double check, due to running big endian on little endian */
	}
}

uint8_t lb( uint32_t rsa, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;
	union ptr2uint d_addr; /* address to access from */
	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory read detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsa);
		return 0; /* return 0 for segfault or other memory error */
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint) >> 2;
		//return (uint8_t)(( data_mem[offset] & 0xFF000000) >> 24); /* May need to double check, due to running big endian on little endian */

		d_addr.uint = dmem_p.uint + offset;
		return  (*((uint8_t *) d_addr.ptr)); /* read from memory*/
		//return (uint8_t)(data_mem[offset]);
	}
}

/* Load Immediate */
uint32_t li( uint16_t imm ){  
	/* Get signed version of immediate */
	return  SEXT_16B( (uint32_t)imm );  
}

uint32_t lsi( uint16_t imm ){ 
	/* Supposed to write to system register file*/
	return  SEXT_16B( (uint32_t)imm );  
}

uint32_t lgi( uint16_t imm ){ 
	/* Supposed to write to global register file*/
	return  SEXT_16B( (uint32_t)imm );  
}

uint32_t lui( uint16_t imm ){ 
	printf("Immediate before lui: %08x\n", (unsigned long) imm);
	printf("Immediate after lui: %08x\n", (unsigned long)(imm << 16));

	return (uint32_t)(imm << 16);
}

uint32_t lusi( uint16_t imm){ 
	/* Supposed to write to system register file*/
	return (uint32_t)imm << 16;
}

uint32_t lugi( uint16_t imm ){ 
	/* Supposed to write to global register file*/
	return (uint32_t)imm << 16;
}

uint32_t lni( uint16_t imm ){ 
	/* Only change lower 2 bytes, save upper */
	return ( (uint32_t)imm & 0x0000FFFF ); 
}

uint32_t lnsi( uint16_t imm ){ 
	/* Supposed to write to system register file*/
	return ( (uint32_t)imm & 0x0000FFFF ); 
}
 
uint32_t lngi( uint16_t imm ){ 
	/* Supposed to write to global register file*/
	return ( (uint32_t)imm & 0x0000FFFF ); 
}

uint32_t luni( uint16_t imm ){ 
	/* Only change upper 2 bytes, save lower */
	return ( (uint32_t)imm << 16 );
}

uint32_t lunsi( uint16_t imm ){ 
	/* only change upper 2 bytes, save lower */
	return ( (uint32_t)imm << 16 );
}
 
uint32_t lungi( uint16_t imm ){ 
	/* only change upper 2 bytes, save lower */
	return ( (uint32_t)imm << 16 );
}


/* Store */
void sw( uint32_t rsa, uint32_t rsb, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;

	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory write detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsb);
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint);
		offset >> 2; ///= 4;
		dmem[offset] = rsb;
	}
}

void sh( uint32_t rsa, uint32_t rsb, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;

	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;
	
	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory write detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsb);
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */

		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint);
		offset >> 2; ///= 4;
		dmem[offset] &= 0x0000FFFF;
		dmem[offset] |= (rsb & 0x0000FFFF) << 16;
	}
}

void sb( uint32_t rsa, uint32_t rsb, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;

	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;
	
	if( (offset > dmem_end_p.uint ) || (offset < ((intptr_t)dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory write detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsb);
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint);
		offset >> 2;///= 4;
		dmem[offset] &= 0x00FFFFFF;
		dmem[offset] |= (rsb & 0x000000FF) << 24;
	}
}

void sth( uint32_t rsa, uint32_t rsb, uint16_t imm ){ 
	uint32_t offset = ( ( rsa + imm ));
	union ptr2uint dmem_p;
	union ptr2uint dmem_end_p;

	dmem_p.ptr = (void *)dmem;
	dmem_end_p.ptr = (void *)dmem_end;

	if( (offset > dmem_end_p.uint ) || (offset < (dmem_p.uint)) ){ /* need to check bounds first, can't access outside memory otherwise segfault */
		/* segfault: writing to memory outside of data segment */
		printf("ERROR:\tSegfault in memory write detected at address %08x.\nValue attempted to write:%08x\n",(unsigned long)offset, (unsigned long)rsb);
	} else {
		/* no issue, can write to memory just fine, so need to get 32 bit
		 * aligned address  */
		offset = ( ( rsa + (uint32_t)( imm & 0x3fff) ) - dmem_p.uint);
		offset /= 4;
		dmem[offset] &= 0x000000FF;
		dmem[offset] |= ( rsb ) << 8; 
	}
}

/* Jump */
void j( uint32_t* pc, uint32_t imm ){ 
	(*pc) = (uint32_t)( (*pc) + SEXT_21B( imm ) + 4 ); /* 21 bit immediate */
}

/* Returns PC to save */
uint32_t jal( uint32_t* pc,  uint32_t imm, uint32_t* ra ){
	(*ra) = (*pc) + 4; /* Save to return address register */
	(*pc) = SEXT_21B( imm ) + 4;
	return (*ra); /* return saved address */
}

void jr( uint32_t* pc, uint32_t rsa, uint32_t imm ){
	if( rsa == 0 )
		j( pc, imm);
	else
		(*pc) = (uint32_t)( (*pc) + (int32_t)rsa + (int32_t)( imm & 0x003FFFFF)); 
}

uint32_t jrl( uint32_t* pc, uint32_t rsa, uint32_t imm, uint32_t* ra){
	if( rsa == 0 ){
		return jal( pc, imm, ra);
	}
	else{
		(*ra) = (*pc) + 4; /* Save to return address register */
		(*pc) += rsa + (int32_t)( imm & 0x003FFFFF); 
		return (*ra);
	}
}


/* Branch */

/* All branch functions return 0 if branch not taken, 1 if taken */
int beq( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm ){
	int taken = ( rsa == rsb );
	(*pc) += ( taken ) ? SEXT_14B( (uint32_t)imm ) : 4;
	return taken;
}

int bne( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm ){
	int taken = ( rsa != rsb );
	(*pc) += ( taken ) ? SEXT_14B( (uint32_t)imm ) : 4;
	return taken;
}

int bgt( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm ){
	int taken = ( ((int32_t)rsa) > ((int32_t)rsb) );
	(*pc) += ( taken ) ? SEXT_14B( (uint32_t)imm ) : 4;
	return taken;
}

int blt( uint32_t* pc, uint32_t rsa, uint32_t rsb, uint16_t imm ){
	int taken = ( ((int32_t)rsa) < ((int32_t)rsb) );
	(*pc) += ( taken ) ? SEXT_14B( (uint32_t)imm ) : 4;
	return taken;
}
