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

typedef struct insn_data{
  const char* name;
  uint8_t rd;
  uint8_t rsa;
  uint8_t rsb;
  uint32_t* data_mem;
  uint32_t* registers;
}insn_details;


/* Executing instruction */

/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//R Type
void add( void ){ 
  registers[rd] = registers[rsa] + registers[rsb];
  insn_name = "add";
  return;
}

void sub( void ){
  registers[rd] =  registers[rsa] - registers[rsb];
  insn_name = "sub";
  return;
}

void not_r( void  ){ 
  registers[rd] = ~registers[rsa];
  insn_name = "not";
  return;
}

void and_r( void ){ 
  registers[rd] = registers[rsa] & registers[rsb];
  insn_name = "and";
  return;
}
void or_r( void ){ 
  registers[rd] = registers[rsa] | registers[rsb];
  insn_name = "or";
  return;
}

void xor_r( void ){
  registers[rd] = registers[rsa] ^ registers[rsb];
  insn_name = "xor";
  return;
}

void sal( void ){
  registers[rd] = registers[rsa] << registers[rsb];
  insn_name = "sal";
  return;
}

void sar( void ){
  registers[rd] = registers[rsa] >> registers[rsb];
  insn_name = "sar";
  return;
}
void sll( void ){
  registers[rd] = registers[rsa] << registers[rsb];
  insn_name = "sll";
  return;
}

void slr( void ){
  registers[rd] = registers[rsa] >> registers[rsb];
  insn_name = "slr";
  return;
}

int32_t comp( void ){
  int32_t a = registers[rsa];
  int32_t b = registers[rsb];
  insn_name = "comp";
  return ( a > b ) ? 1:(( a < b ) ? -1:0 );
}



/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//I Type
void addi( void ){ 
  registers[rd] = registers[rsa] + imm;
  insn_name = "addi";
  return;
}


void subi( void ){ 
  registers[rd] = registers[rsa] - imm;
  insn_name = "sub";
  return;
}
 

void not_i( void  ){ 
  registers[rd] = ~imm;
  insn_name = "not";
  return;
}

void and_i( void ){ 
  registers[rd] = registers[rsa] & imm;
  insn_name = "and";
  return;
}
void or_i( void ){ 
  registers[rd] = registers[rsa] || imm;
  insn_name = "or";
  return;
}

void xor_i( void ){
  registers[rd] = registers[rsa] ^ imm;
  insn_name = "xor";
  return;
}

void sali( void ){
  registers[rd] = registers[rsa] << imm;
  insn_name = "sal";
  return;
}

void sari( void ){
  registers[rd] = registers[rsa] >> imm;
  insn_name = "sar";
  return;
}
void slli( void ){
  registers[rd] = registers[rsa] << imm;
  insn_name = "sll";
  return;
}

void slri( void ){
  registers[rd] = registers[rsa] >> imm;
  insn_name = "slr";
  return;
}

int32_t compi( void ){
  int32_t a = registers[rsa];
  int32_t b = imm;
  insn_name = "comp";
  return ( a > b ) ? 1:(( a < b ) ? -1:0 );
}



//Load Type
void lw( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] = data_mem[idx];
  //registers[rd] = ((data_mem[idx]<< 24) & 0xFF000000) |  ((data_mem[idx+1] << 16) & 0x00FF0000) |  ((data_mem[idx+2] << 8) &  0x0000FF00) | (data_mem[idx+3] & 0x000000FF); 
  insn_name = "li";
  return;
}

void lth( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] = data_mem[idx] & 0x00FFFFFF;
  //registers[rd] = ((data_mem[idx] << 16) & 0x00FF0000) |  ((data_mem[idx+1] << 8) &  0x0000FF00) | (data_mem[idx+2] & 0x000000FF);
  insn_name = "lth";
  return;
}

void lh( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] = data_mem[idx] & 0x0000FFFF;
  //registers[rd] = ((data_mem[idx] << 8) &  0x0000FF00) | (data_mem[idx+1] & 0x000000FF);
  insn_name = "lh";
  return; 
}

void lb( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] =  (data_mem[idx] & 0x000000FF);
  insn_name = "lb";
  return;
}





/*	* 	*	 * 	* 	*	 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
//LI Type
void li( void ){  
  registers[rd] &= 0x0000FFFF;
  registers[rd] =  (((int)imm << 16) >> 16 );  
  insn_name = "lw";
  return; 
}


void lsi( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] = (((int)imm << 16) >> 16); 
  insn_name = "lsi";
  return;
}


void lgi( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] = (((int)imm << 16) >> 16); 
  insn_name = "lgi";
  return;
}


void lui( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= (( imm << 16 ) & 0xFFFF0000);
  insn_name = "lui";
  return;
}


void lusi( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= (int32_t)(( imm << 16 ) & 0xFFFF0000); 
  insn_name = "lusi";
  return;
}


void lugi( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= (( imm << 16 ) & 0xFFFF0000);
  insn_name = "lugi";
  return;
}


void lni( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] = (imm & 0x0000FFFF); 
  insn_name = "lni"; 
  return; 
}


void lnsi( void ){ 
  registers[rd] = (imm & 0x0000FFFF); 
  insn_name = "lnsi";
  return;
}

 
void lngi( void ){ 
  registers[rd] = (imm & 0x0000FFFF); 
  insn_name = "lngi";
  return;
}


void luni( void ){ 
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= ( imm << 16 );
  insn_name = "luni";
  return;
}


void lunsi( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= ( data_mem[idx] << 16 );
  insn_name = "linsi";
  return;
}

 
void lungi( void ){ 
  uint32_t idx = registers[rsa] + imm;
  registers[rd] &= 0x0000FFFF;
  registers[rd] |= ( data_mem[idx] << 16 );
  insn_name = "lungi";
  return;
}



//S Type
void sw( void ){ 
  uint32_t idx = registers[rsa] + imm;
  data_mem[idx] = registers[rsb];
  /*
  data_mem[idx  ] =  (registers[rsb] >> 24) & 0x000000FF;
  data_mem[idx+1] =  (registers[rsb] >> 16) & 0x000000FF;
  data_mem[idx+2] =  (registers[rsb] >>  8) & 0x000000FF;
  data_mem[idx+3] =  (registers[rsb]      ) & 0x000000FF;
  insn_name = "sw";
  */
  return;
}

void sh( void ){ 
  uint32_t idx = registers[rsa] + imm;
  data_mem[idx] = registers[rsb] & 0x0000FFFF;
  /*
  data_mem[idx  ] =  (registers[rsb] >>  8) & 0x000000FF;
  data_mem[idx+1] =  (registers[rsb]      ) & 0x000000FF;
  insn_name = "sh";
  */
  return;
}

void sb( void ){ 
  uint32_t idx = registers[rsa] + imm;
  data_mem[idx] = registers[rsb] & 0x000000FF;
  /*
  data_mem[idx] = (registers[rsb] & 0x000000FF);
  insn_name = "sb";
  */
  return;
}


void sth( void ){ 
  uint32_t idx = registers[rsa] + imm;
  data_mem[idx] = registers[rsb] & 0x00FFFFFF;
  /*
  data_mem[idx  ] =  (registers[rsb] >> 16) & 0x000000FF;
  data_mem[idx+1] =  (registers[rsb] >>  8) & 0x000000FF;
  data_mem[idx+2] =  (registers[rsb]      ) & 0x000000FF;
  insn_name = "sth";
  */
  return;
}



//J Type
void j( void ){ 
  PC += imm/4 - 1;
  insn_name = "j";
  return;
}

void jal( void ){
  //registers[rsa] = PC + 1;
  PC += imm/4 - 1;
  //std::cout << "PC = " << PC << "\t IMM = " << BASE_MEM - imm << std::endl;
  insn_name = "jal";
  return;
}


void jr( void ){
  //PC += (registers[rsa] + imm)/4 - 1; 
  PC += ( + imm)/4 - 1; 
  //cout << " At jr PC = " << PC << " imm = " << imm << " reg[rsa] = " << registers[rsa] << endl;
  insn_name = "jr";
  return;
}

void jrl( void ){
  //registers[RA1] = PC + 1;
  PC += ( + imm)/4 - 2; 
  //PC += (registers[rsa] + imm)/4 - 1;
//  std::cout << "PC = " << PC << "\t IMM = " << imm << std::endl;
  insn_name = "jrl";
  return;
}



//B Type
void beq(void){
  int32_t a = ((((int)registers[rsa]) << 16) >> 16 );
  int32_t b = ((((int)registers[rsb]) << 16) >> 16 );
  int imm_t = ((((int)imm) << 19) >> 19);
  PC = (a == b) ? (PC+imm_t/4 - 1) : PC;
  insn_name = "beq";
  return;
}


void bne( void ){
  int32_t a = ((((int)registers[rsa]) << 16) >> 16 );
  int32_t b = ((((int)registers[rsb]) << 16) >> 16 );
  int imm_t = ((((int)imm) << 19) >> 19);
  //cout << " Condition: " << (a != b) << "        Imm: " << imm_t << endl;
  PC = (a != b) ? (PC+imm_t/4 - 1) : PC;
  insn_name = "bne";
  return;
}



void bgt( void ){
  int32_t a = ((((int)registers[rsa]) << 16) >> 16 );
  int32_t b = ((((int)registers[rsb]) << 16) >> 16 );
  int imm_t = ((((int)imm) << 19) >> 19); 
  PC = (a > b) ? (PC+imm_t/4 - 1) : PC;
  insn_name = "bgt";
  return;
}



void blt( void ){
  int32_t a = ((((int)registers[rsa]) << 16) >> 16 );
  int32_t b = ((((int)registers[rsb]) << 16) >> 16 );
  int imm_t = ((((int)imm) << 19) >> 19); 
  PC = (a < b) ? (PC+imm_t/4 - 1) : PC;
  insn_name = "blt";
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


 void write_to_disk( const char* filename ){
  FILE *file;
  file = fopen(filename, "w+");
  for( unsigned int i = 0; (i < DMEM_SIZE); i++)
    fprintf(file, "%0x: %0x\n", BASE_MEM + i, data_mem[i]);
  fclose(file);

  return;
}











void execute( uint32_t* PC, uint32_t* pc ){
    //printf("%08x:\t %08x", pc, insn);

   // uint8_t idx = (pc - entry_point)/4;
    uint32_t insn =  insn_mem[*PC];
    //printf("%08x:\t %08x", pc, insn);
    if( IS_R_TYPE( insn ) ){
      insn_type = "R";
      rd = GET_RD_R(insn); 
      rsa = GET_RSA_R(insn);
      rsb = GET_RSB_R(insn);
   
      switch( GET_FUNCT_R(insn) ){
        case(0x0): add(); break;
        case(0x1): sub( ); break;
        case(0x4): not_r(); break;
        case(0x5): and_r(); break;
        case(0x6): or_r(); break;
        case(0x7): xor_r(); break;
        case(0x8): sal(); break;
        case(0x9): sar(); break;
        case(0xA): sll(); break;
        case(0xB): slr(); break;
        case(0xC): comp(); break;
        default: ;//printf( "Not valid R Type instruction\n");
     }
   
    }
    else if( IS_I_TYPE( insn ) ){
      insn_type = "Immediate"; 
      rd = GET_RD_I(insn); 
      rsa = GET_RSA_I(insn);
      imm = GET_IMM_I(insn);
    
      switch( GET_FUNCT_I(insn) ){
        case(0x0): addi(); break;
        case(0x1): subi(); break;
        case(0x4): not_i(); break;
        case(0x5): and_i(); break;
        case(0x6): or_i(); break;
        case(0x7): xor_i(); break;
        case(0x8): sali(); break;
        case(0x9): sari(); break;
        case(0xA): slli(); break;
        case(0xB): slri(); break;
        case(0xC): compi(); break;
        default:; //printf("Not valid I Type instruction\n");
     }
    
    }
    else if( IS_L_TYPE( insn ) ){
      insn_type = "Load";
      rd = GET_RD_L(insn);
      rsa = GET_RSA_L(insn);
      imm = GET_IMM_L(insn);

      switch( GET_FUNCT_L(insn) ){
        case(0x0): lw(); break;
        case(0x1): lh(); break;
        case(0x3): lb(); break;
        case(0x2): lth(); break;
        default:; //printf("Not valid L Type instruction\n");
      }
    }
    else if( IS_LI_TYPE( insn ) ){
      insn_type = "Load Immediate";
      rd = GET_RD_LI(insn);
      imm = GET_IMM_LI(insn);
    
      switch( GET_FUNCT_LI(insn) ){
        case(0x0): li(); break;
        case(0x1): lsi(); break;
        case(0x2): lgi(); break;
        case(0x3): lui(); break;
        case(0x4): lusi(); break;
        case(0x5): lugi(); break;
        case(0x8): lni(); break;
        case(0x9): lnsi(); break;
        case(0xA): lngi(); break;
        case(0xB): luni(); break;
        case(0xC): lunsi(); break;
        case(0xD): lungi(); break;
        default: ;//printf("Not valid LI Type instruction\n");
      }
    }
    else if( IS_S_TYPE( insn ) ){
      insn_type = "Store";
      rsa = GET_RSA_S(insn);
      rsb = GET_RSB_S(insn);
      imm = GET_IMM_S(insn);

      switch( GET_FUNCT_S(insn) ){
        case(0x0): sw(); break;
        case(0x1): sh(); break;
        case(0x3): sb(); break;
        case(0x2): sth(); break;
        default: ;//printf( "Not valid S Type instruction\n");
      } 
    
    }
    else if( IS_J_TYPE( insn ) ){
      insn_type = "Jump";
      rsa = GET_RSA_J(insn);
      imm = GET_IMM_J(insn);

      switch( GET_FUNCT_J(insn) ){
        case(0x0): j(); break;
        default: jr(); break;;
      }
         
   
    }
    else if( IS_JL_TYPE( insn ) ){
      insn_type = "Jump and Link";
      rsa = GET_RSA_J(insn);
      imm = GET_IMM_J(insn);

      switch( GET_FUNCT_J(insn) ){
        case(0x0): jal(); break;
        default: jrl(); break;
      }
    } 
    else if( IS_B_TYPE( insn ) ){
      insn_type = "Branch";
      rsa = GET_RSA_B(insn);
      rsb = GET_RSB_B(insn);
      imm = GET_IMM_B(insn);
      switch( GET_FUNCT_B(insn) ){
        case(0x0): beq(); break;
        case(0x1): bne(); break;
        case(0x2): bgt(); break;
        case(0x3): blt(); break;
        default: ;//printf("Not valid B Type instruction\n");
      }
   
    }
    else if( IS_SYS_TYPE( insn ) ){
      insn_type = "System";
      //printf("0x%0x\n", insn );
    }
  
    else if( IS_CO_TYPE( insn ) ){
     //printf("Co Type Instruction\n");
    }
    else if( IS_NOP( insn ) ){
      //printf("No operation\n");
    }
    else{
      //printf("Unknown instruction\n");
    }

   // print_insn_details();
    (*PC)++;
     *pc = entry_point + (*PC) * 4;
  

  return;
}

