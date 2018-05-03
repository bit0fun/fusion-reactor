#include "fusion-core-sim.h"

#ifndef NUM_REGS
#define NUM_REGS 32
#endif
#ifndef DMEM_SIZE
#define DMEM_SIZE 2^(14)
#endif

#ifndef BASE_MEM
#define BASE_MEM 0x40000 /* Struct for passsing arguments to function*/
#endif

typedef struct {
  char* name;//[8]; /* instruction names longer than this aren't possible */
  char type;
  uint8_t rd;
  uint8_t rsa;
  uint8_t rsb;
  uint32_t imm;
} insn_info;


/* Executing instruction */

/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//R Type
void add( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[ insn->rd ] = (*registers)[ insn->rsa ] + (*registers)[ insn->rsb ];
  insn->name = "add";
  return;
}

void sub( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] =  (*registers)[insn->rsa] - (*registers)[insn->rsb];
  insn->name = "sub";
  return;
}

void not_r( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = ~( (*registers)[insn->rsa] );
  insn->name = "not";
  return;
}

void and_r( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] & (*registers)[insn->rsb];
  insn->name = "and";
  return;
}
void or_r(insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] | (*registers)[insn->rsb];
  insn->name = "or";
  return;
}

void xor_r(insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] ^ (*registers)[insn->rsb];
  insn->name = "xor";
  return;
}

void sal(insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] << (*registers)[insn->rsb];
  insn->name = "sal";
  return;
}

void sar(insn_info* insn, uint32_t* registers[32] ){
	/* Since the C defintion for shifting is undefined, need to do something
	 * special */
  (*registers)[insn->rd] = (*registers)[insn->rsa] >> (*registers)[insn->rsb];
  /* Determining if signed*/
  if( 0x80000000 & (*registers)[insn->rsa] ) {
	  (*registers)[insn->rd] |= 0x80000000; /* Ensuring signed properly, set MSB */
  } else {
  	  (*registers)[insn->rd] &= 0x7fffffff; /* Ensuring signed properly, clear MSB */
  }
  insn->name = "sar";
  return;
}
void sll(insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] << (*registers)[insn->rsb];
  insn->name = "sll";
  return;
}

void slr(insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] >> (*registers)[insn->rsb];
  insn->name = "slr";
  return;
}

int32_t comp( insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = (*registers)[insn->rsb];
  insn->name = "comp";
  return ( a > b ) ? 1:(( a < b ) ? -1:0 );
}



/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//I Type
void addi( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] + insn->imm;
  insn->name = "addi";
  return;
}


void subi( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] - insn->imm;
  insn->name = "sub";
  return;
}
 

void not_i( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = ~(insn->imm);
  insn->name = "not";
  return;
}

void and_i( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] & (insn->imm);
  insn->name = "and";
  return;
}
void or_i( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] = (*registers)[insn->rsa] | insn->imm;
  insn->name = "or";
  return;
}

void xor_i( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] ^ (insn->imm);
  insn->name = "xor";
  return;
}

void sali( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] << (insn->imm);
  insn->name = "sal";
  return;
}

void sari( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] >> (insn->imm);
  if( 0x80000000 & (*registers)[insn->rsa] ) {
	  (*registers)[insn->rd] |= 0x80000000; /* Ensuring signed properly, set MSB */
  } else {
  	  (*registers)[insn->rd] &= 0x7fffffff; /* Ensuring signed properly, clear MSB */
  }
  insn->name = "sar";
  return;
}
void slli( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] << (insn->imm);
  insn->name = "sll";
  return;
}

void slri( insn_info* insn, uint32_t* registers[32] ){
  (*registers)[insn->rd] = (*registers)[insn->rsa] >> (insn->imm);
  insn->name = "slr";
  return;
}

int32_t compi( insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = insn->imm;
  insn->name = "comp";
  return ( a > b ) ? 1:(( a < b ) ? -1:0 );
}



//Load Type
void lw( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  (*registers)[insn->rd] = data_mem[idx];
  insn->name = "lw";
  return;
}

void lth( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  (*registers)[insn->rd] = (data_mem[idx] & 0xFFFFFF00) >> 8; /* May need to double check, due to running big endian on little endian */
  insn->name = "lth";
  return;
}

void lh( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x0000ffff) ) - dseg_i.start)/4 ;
  (*registers)[insn->rd] = (data_mem[idx] & 0xFFFF0000) >> 16; /* May need to double check, due to running big endian on little endian */
  insn->name = "lh";
  return; 
}

void lb( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x0000ffff) ) - dseg_i.start)/4 ;
  (*registers)[insn->rd] = (data_mem[idx] & 0xFF000000) >> 24; /* May need to double check, due to running big endian on little endian */
  insn->name = "lb";
  return;
}





/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//LI Type
void li( insn_info* insn, uint32_t* registers[32] ){  
	/* Get signed version of immediate */
	int16_t sign_imm = (int16_t) (insn->imm & 0x0000ffff);   
	(*registers)[insn->rd] = (( sign_imm << 16) >> 16 );  
	insn->name = "li";
	return; 
}


void lsi( insn_info* insn, uint32_t* registers[32] ){ 
	int16_t sign_imm = (int16_t) (insn->imm & 0x0000ffff);   
	//registers[insn->rd] = (( sign_imm << 16) >> 16 );  
	/* Supposed to write to system register file, so do nothing at the moment*/
	insn->name = "lsi";
  return;
}


void lgi( insn_info* insn, uint32_t* registers[32] ){ 
	int16_t sign_imm = (int16_t) (insn->imm & 0x0000ffff);   
	//registers[insn->rd] = (( sign_imm << 16) >> 16 );  
	/* Supposed to write to system register file, so do nothing at the moment*/
  	insn->name = "lgi";
  	return;
}


void lui( insn_info* insn, uint32_t* registers[32] ){ 
//  registers[rd] &= 0x0000FFFF;
  (*registers)[insn->rd] = ( insn->imm << 16 ) & 0xFFFF0000;
  insn->name = "lui";
  return;
}


void lusi( insn_info* insn, uint32_t* registers[32] ){ 
/*
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= ( imm << 16 ); */
	/* Do nothing until implementation of system register file */
  insn->name = "lusi";
  return;
}


void lugi( insn_info* insn, uint32_t* registers[32] ){ 
/*
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= (( imm << 16 ) & 0xFFFF0000);
  */
	/* Do nothing until implementation of global register file */
  insn->name = "lugi";
  return;
}


void lni( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] &= 0xFFFF0000;
  (*registers)[insn->rd] |= (insn->imm & 0x0000FFFF); 
  insn->name = "lni"; 
  return; 
}


void lnsi( insn_info* insn, uint32_t* registers[32] ){ 
//  registers[insn->rd] = (imm & 0x0000FFFF); 
	/* Do nothing until implementation of system register file */
  insn->name = "lnsi";
  return;
}

 
void lngi( insn_info* insn, uint32_t* registers[32] ){ 
 // registers[insn->rd] = (insn->imm & 0x0000FFFF); 
	/* Do nothing until implementation of global register file */
  insn->name = "lngi";
  return;
}


void luni( insn_info* insn, uint32_t* registers[32] ){ 
  (*registers)[insn->rd] &= 0x0000FFFF;
  (*registers)[insn->rd] |= ( insn->imm << 16 );
  insn->name = "luni";
  return;
}


void lunsi( insn_info* insn, uint32_t* registers[32] ){ 
/*  registers[insn->rd] &= 0x0000FFFF;
  registers[insn->rd] |= ( imm << 16 ); */
	/* Do nothing until implementation of system register file */
  insn->name = "lunsi";
  return;
}

 
void lungi( insn_info* insn, uint32_t* registers[32] ){ 
/*  registers[insn->rd] &= 0x0000FFFF;
  registers[insn->rd] |= ( imm << 16 ); */
	/* Do nothing until implementation of global register file */
  insn->name = "lungi";
  return;
}



//S Type
void sw( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  data_mem[idx] = (*registers)[insn->rsb];
  insn->name = "sw";
  return;
}

void sh( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  data_mem[idx] &= 0x0000FFFF;
  data_mem[idx] |= ((*registers)[insn->rsb]) << 16;
  insn->name = "sh";
  return;
}

void sb( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( *(registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  data_mem[idx] &= 0x000000FF;
  data_mem[idx] |= ((*registers)[insn->rsb]) << 24;
  insn->name = "sb";
  return;
}


void sth( uint32_t* data_mem, dataseg_info dseg_i, insn_info* insn, uint32_t* registers[32] ){ 
  uint32_t idx = (( (*registers)[insn->rsa] + (insn->imm & 0x00003fff) ) - dseg_i.start)/4 ;
  data_mem[idx] &= 0x000000FF;
  data_mem[idx] |= ((*registers)[insn->rsb]) << 24; 
  insn->name = "sth";
  return;
}



//J Type
void j( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){ 
  (*pc) += ( insn->imm ) & 0x003FFFFF; /* 21 bit immediate */
  insn->name = "j";
  return;
}

void jal( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  (*registers)[4] = (*pc) + 4; /* Save to return address register */
  (*pc) += ( insn->imm ) & 0x003FFFFF;
  insn->name = "jal";
  return;
}


void jr( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  (*pc) += (*registers)[insn->rsa] + ( ( insn->imm ) & 0x003FFFFF); 
  insn->name = "jr";
  return;
}

void jrl( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
	(*registers)[4] = (*pc) + 4; /* Save to return address register */
	(*pc) += (*registers)[insn->rsa] + ( ( insn->imm ) & 0x003FFFFF); 
  insn->name = "jrl";
  return;
}



//B Type
void beq( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = (*registers)[insn->rsb];
  int16_t signed_imm = ((((signed int)insn->imm) << 19) >> 19);
  (*pc) += (a == b) ? ( signed_imm ) : 4;
  insn->name = "beq";
  return;
}


void bne( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = (*registers)[insn->rsb];
  int16_t  signed_imm = ((((signed int)insn->imm) << 19) >> 19);
  (*pc) += (a != b) ? ( signed_imm ) : 4;
  insn->name = "bne";
  return;
}



void bgt( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = (*registers)[insn->rsb];
  int16_t signed_imm  = ((((signed int)insn->imm) << 19) >> 19);
  (*pc) += (a > b) ? ( signed_imm ) : 4;
  insn->name = "bgt";
  return;
}



void blt( uint32_t* pc, insn_info* insn, uint32_t* registers[32] ){
  int32_t a = (*registers)[insn->rsa];
  int32_t b = (*registers)[insn->rsb];
  int16_t  signed_imm  = ((((signed int)insn->imm) << 19) >> 19);
  (*pc) += (a < b) ? ( signed_imm ) : 4;
  insn->name = "blt";
  return;
}

/*
void print_insn_details( void ){
  printf("Instruction type: %s\n", insn_type.c_str());
  printf("Instriuction: %s\n", insn_name.c_str());
  if( !insn_type.compare("R") ){
    printf("RD: %d\n", rd);
    printf("RSA: %d\n", rsa);
    printf("RSB: %d\n", rsb);
  }
  if( !insn_type.compare("Immediate") ){
    printf("RD: %d\n", rd);
    printf("RSA: %d\n", rsa);
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Load Immediate") ){
    printf("RD: %d\n", rd);
    printf("RSA: %d\n", rsa);
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Load") ){
    printf("RD: %d\n", rd);
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Jump") ){
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Jump and Link") ){
    printf("RSA: %d\n", rsa);
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Branch") ){
    printf("RSA: %d\n", rsa);
    printf("IMM: %d\n", imm);
  }
  if( !insn_type.compare("Jump and Link") ){
    printf("RSA: %d\n", rsa);
    printf("IMM: %d\n", imm);
  }
  printf("\n\n");
  for (int i = 0; i < NUM_REGS; i+=2){                                                        
    printf("\t Register[%02d]: 0x%08x \t Register[%02d]: 0x%08x\n", i, registers[i], i+1, registers[i+1]);
  }
  printf("\n\n");
  printf("PC: 0x%0x\n\n", INSN_MEM_BASE + PC*4);

  return;
}
*/


 void write_to_disk( const char* filename, uint32_t* data_mem){
  FILE *file;
  file = fopen(filename, "w+");
  for( unsigned int i = 0; (i < DMEM_SIZE); i++)
    fprintf(file, "%0x: %0x\n", BASE_MEM + i, data_mem[i]);
  fclose(file);

  return;
}

int execute( uint32_t* pc, uint32_t** tseg, textseg_info* tseg_i, uint32_t** data_mem, dataseg_info* dseg_i, uint32_t* registers[32] ){
	/* Retrieving text segment data from struct */
	uint32_t text_start = tseg_i->start;
	uint32_t text_size	= tseg_i->size;
	/* Takes text segment and gets the instruction */
	uint32_t insn		= (*tseg)[ ( (*pc) - text_start ) / 4  ];
	insn_info insn_i;

	/* Determining Instruction Type */
    if( IS_R_TYPE( insn ) ){
      //insn_i.type = 'R'; /* not that needed */
      insn_i.rd = GET_RD_R(insn); 
      insn_i.rsa = GET_RSA_R(insn);
      insn_i.rsb = GET_RSB_R(insn);
   
      switch( GET_FUNCT_R(insn) ){
        case(0x0): add( &insn_i, registers ); break;
        case(0x1): sub( &insn_i, registers ); break;
        case(0x4): not_r( &insn_i, registers ); break;
        case(0x5): and_r( &insn_i, registers ); break;
        case(0x6): or_r( &insn_i, registers ); break;
        case(0x7): xor_r( &insn_i, registers ); break;
        case(0x8): sal( &insn_i, registers ); break;
        case(0x9): sar( &insn_i, registers ); break;
        case(0xA): sll( &insn_i, registers ); break;
        case(0xB): slr( &insn_i, registers ); break;
        case(0xC): comp( &insn_i, registers ); break;
        default:
			return -1; /* Invalid instruction */
	  }
   
    	(*pc)+=4;
    }
    else if( IS_I_TYPE( insn ) ){
      //insn_i.type = ;
      insn_i.rd = GET_RD_R(insn); 
      insn_i.rsa = GET_RSA_R(insn);
      insn_i.imm = GET_IMM_I(insn);
    
      switch( GET_FUNCT_I(insn) ){
        case(0x0): addi( &insn_i, registers ); break;
        case(0x1): subi( &insn_i, registers ); break;
        case(0x4): not_i( &insn_i, registers ); break;
        case(0x5): and_i( &insn_i, registers ); break;
        case(0x6): or_i( &insn_i, registers ); break;
        case(0x7): xor_i( &insn_i, registers ); break;
        case(0x8): sali( &insn_i, registers ); break;
        case(0x9): sari( &insn_i, registers ); break;
        case(0xA): slli( &insn_i, registers ); break;
        case(0xB): slri( &insn_i, registers ); break;
        case(0xC): compi( &insn_i, registers ); break;
        default:
			return -1; /* Invalid instruction */
     }
    
    	(*pc)+=4;
    }
    else if( IS_L_TYPE( insn ) ){
      insn_i.rd = GET_RD_R(insn); 
      insn_i.rsa = GET_RSA_R(insn);
      insn_i.imm = GET_IMM_I(insn);


      switch( GET_FUNCT_L(insn) ){
 		  case(0x0): lw( *data_mem, *dseg_i, &insn_i, registers ); break;
    	  case(0x1): lh( *data_mem, *dseg_i, &insn_i, registers ); break;
		  case(0x3): lb( *data_mem, *dseg_i, &insn_i, registers ); break;
		  case(0x2): lth( *data_mem, *dseg_i, &insn_i, registers ); break;
		  default:
			return -1; /* Invalid instruction */
      }
      (*pc)+=4;
    }
    else if( IS_LI_TYPE( insn ) ){
      insn_i.rd = GET_RD_LI(insn);
      insn_i.imm = GET_IMM_LI(insn);

    
      switch( GET_FUNCT_LI(insn) ){
        case(0x0): li( &insn_i, registers ); break;
        case(0x1): lsi( &insn_i, registers ); break;
        case(0x2): lgi( &insn_i, registers ); break;
        case(0x3): lui( &insn_i, registers ); break;
        case(0x4): lusi( &insn_i, registers ); break;
        case(0x5): lugi( &insn_i, registers ); break;
        case(0x8): lni( &insn_i, registers ); break;
        case(0x9): lnsi( &insn_i, registers ); break;
        case(0xA): lngi( &insn_i, registers ); break;
        case(0xB): luni( &insn_i, registers ); break;
        case(0xC): lunsi( &insn_i, registers ); break;
        case(0xD): lungi( &insn_i, registers ); break;
        default: 
			return -1; /* Ivalid Instruction */
      }
		(*pc) += 4;
    }
    else if( IS_S_TYPE( insn ) ){
      //insn_type = "Store";
      insn_i.rsa = GET_RSA_S(insn);
      insn_i.rsb = GET_RSB_S(insn);
      insn_i.imm = GET_IMM_S(insn);

      switch( GET_FUNCT_S(insn) ){
        case(0x0): sw( *data_mem, *dseg_i, &insn_i, registers ); break;
        case(0x1): sh( *data_mem, *dseg_i, &insn_i, registers ); break;
        case(0x3): sb( *data_mem, *dseg_i, &insn_i, registers ); break;
        case(0x2): sth( *data_mem, *dseg_i, &insn_i, registers ); break;
        default:
			return -1; /* Invalid Instruction */
      } 
	  (*pc) += 4;
    }
    else if( IS_J_TYPE( insn ) ){
      //insn_type = "Jump";
	  insn_i.rsa = GET_RSA_J(insn);
      insn_i.imm = GET_IMM_J(insn);

      switch( GET_FUNCT_J(insn) ){
        case(0x0): j( pc, &insn_i, registers); break;
        default: jr( pc, &insn_i, registers); break;;
      }
         
   
    }
    else if( IS_JL_TYPE( insn ) ){
     // insn_type = "Jump and Link";
      insn_i.rsa = GET_RSA_J(insn);
      insn_i.imm = GET_IMM_J(insn);

      switch( GET_FUNCT_J(insn) ){
        case(0x0): jal( pc, &insn_i, registers); break;
        default: jrl( pc, &insn_i, registers); break;
      }
    } 
    else if( IS_B_TYPE( insn ) ){
      //insn_type = "Branch";
      insn_i.rsa = GET_RSA_B(insn);
      insn_i.rsb = GET_RSB_B(insn);
      insn_i.imm = GET_IMM_B(insn);
      switch( GET_FUNCT_B(insn) ){
        case(0x0): beq( pc, &insn_i, registers); break;
        case(0x1): bne( pc, &insn_i, registers); break;
        case(0x2): bgt( pc, &insn_i, registers); break;
        case(0x3): blt( pc, &insn_i, registers); break;
        default: ;//printf("Not valid B Type instruction\n");
      }
   
    }
    else if( IS_SYS_TYPE( insn ) ){
      //insn_type = "System";
      //printf("0x%0x\n", insn );
    }
  
    else if( IS_CO_TYPE( insn ) ){
     //printf("Co Type Instruction\n");
		return 1; /* CO-Processor instructions not implemented yet */
    }
    else if( IS_NOP( insn ) ){
      //printf("No operation\n");
    }
    else{
      //printf("Unknown instruction\n");
	  
		/* Acts like no-op, since can be random bits */
    }
  (*pc) += 4;
  return 0; /* No issue with execution */
}

