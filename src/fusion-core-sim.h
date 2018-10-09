#ifndef FUSION_CORE_SIM_H
#define FUSION_CORE_SIM_H

//#include <cstdint>
//#include <iostream>
//#include <elfio/elfio.hpp>

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Simulation Parameters */
#define DMEM_SIZE	2^(14)	/* Data memory size (kb) for simulation */
#define NUM_REGS 32
#define BASE_MEM 0x400000
#define INSN_MEM_BASE 0x40000

/* Determine Instruction type */
#ifndef IS_R_TYPE
#define IS_R_TYPE(insn) 	( (insn >> 26) == 0x13 )
#endif 
#ifndef IS_I_TYPE
#define IS_I_TYPE(insn) 	( (insn >> 26) == 0x16 )
#endif
#ifndef IS_L_TYPE
#define IS_L_TYPE(insn) 	( (insn >> 26) == 0x1E )
#endif
#ifndef IS_LI_TYPE
#define IS_LI_TYPE(insn) 	( (insn >> 26) == 0x10 )
#endif
#ifndef IS_S_TYPE
#define IS_S_TYPE(insn) 	( (insn >> 26) == 0x1D )
#endif
#ifndef IS_J_TYPE 
#define IS_J_TYPE(insn) 	( (insn >> 26) == 0x0C )
#endif
#ifndef IS_JL_TYPE
#define IS_JL_TYPE(insn) 	( (insn >> 26) == 0x04 )
#endif
#ifndef IS_B_TYPE
#define IS_B_TYPE(insn) 	( (insn >> 26) == 0x0D )
#endif
#ifndef IS_SYS_TYPE
#define IS_SYS_TYPE(insn) 	( (insn >> 26) == 0x18 )
#endif
#ifndef IS_CO_TYPE
#define IS_CO_TYPE(insn) 	( (insn >> 26) == 0x3F )
#endif
#ifndef IS_NOP
#define IS_NOP(insn) 	( (insn >> 26) == 0x00 )
#endif


#ifndef GET_RD_R 
#define GET_RD_R(x) ( (x >> 21) & 0x1F )
#endif
#ifndef GET_RSA_R
#define GET_RSA_R(x) ( (x >> 16) & 0x1F )
#endif
#ifndef GET_RSB_R
#define GET_RSB_R(x) ( (x >> 11) & 0x1F )
#endif
#ifndef GET_FUNCT_R 
#define GET_FUNCT_R(x)  ( x & 0xF )
#endif


#ifndef GET_RD_I
#define GET_RD_I(x) ( (x >> 21) & 0x1F )
#endif
#ifndef GET_RSA_I
#define GET_RSA_I(x) ( (x >> 16) & 0x1F )
#endif
#ifndef GET_IMM_I
#define GET_IMM_I(x) ( (x >> 4) & 0xFFF )
#endif
#ifndef GET_FUNCT_I
#define GET_FUNCT_I(x) (x & 0xF ) 
#endif


#ifndef GET_RD_L  
#define GET_RD_L(x) ( (x >> 21) & 0x1F )
#endif
#ifndef GET_RSA_L
#define GET_RSA_L(x) ( (x >> 16) & 0x1F ) 
#endif
#ifndef GET_FUNCT_L
#define GET_FUNCT_L(x) ( (x >> 14) & 0X3 )
#endif
#ifndef GET_IMM_L
#define GET_IMM_L(x) (x & 0x3FFF)
#endif


#ifndef GET_RD_LI
#define GET_RD_LI(x) ( (x >> 21) & 0x1F )
#endif
#ifndef GET_IMM_LI
#define GET_IMM_LI(x) ( x & 0XFFFF ) 
#endif
#ifndef GET_FUNCT_LI
#define GET_FUNCT_LI(x) ( (x >> 17) & 0xF )
#endif



#ifndef GET_RSA_S
#define GET_RSA_S(x) ( (x >> 16) & 0x1F )
#endif
#ifndef GET_RSB_S
#define GET_RSB_S(x) ( (x >> 11) & 0x1F )
#endif
#ifndef GET_IMM_S
#define GET_IMM_S(x) ( ( x & 0xE00000) >> 10 ) | ( x & 0X7FF )
#endif
#ifndef GET_FUNCT_S
#define GET_FUNCT_S(x) (( x >> 24 ) & 0x3 )
#endif


//#define GET_RSB_J(x) ( (x >> 16) & 0x1F )
//#define GET_IMM_J(x) ( ( x & 0x1F00000) >> 5 ) | ( x & 0XFFFF )
//#define GET_FUNCT_J(x) ( (x >> 26) & 0x3F )

#ifndef GET_RSB_A
#define GET_RSA_B(x) ( ( x >> 16) & 0x1F )
#endif
#ifndef GET_RSB_B
#define GET_RSB_B(x) ( ( x >> 11) & 0x1F )
#endif
#ifndef GET_IMM_B
#define GET_IMM_B(x) ( ( x & 0x3E00000) >> 12 ) | ( (x >> 2) & 0X1FF )
#endif
#ifndef GET_FUNCT_B
#define GET_FUNCT_B(x) ( x & 0x3 )
#endif
    

#ifndef  GET_RSA_J
#define GET_RSA_J(x) ( ( x >> 16) & 0x1F )
#endif
#ifndef  GET_IMM_J
#define GET_IMM_J(x) ( ( x & 0x3E00000) >> 5 ) | ( x & 0XFFFF )
#endif
#ifndef  GET_FUNCT_J
#define GET_FUNCT_J(x) ( (x >> 16) & 0x1F )
#endif



#define ZERE 0,
#define SP 1
#define FP 2
#define GP 3
#define RA 4
#define ARG0 5
#define ARG1 6
#define ARG2 7
#define ARG3 8 
#define RVAL0 9
#define RVAL1 10
#define GR0 11
#define GR1 12
#define GR2 13
#define GR3 14
#define GR4 15
#define GR5 16
#define GR6 17 
#define GR7 18
#define GR8 19
#define GR9 20
#define GR10 21
#define TMP0 22
#define TMP1 23
#define TMP2 24
#define TMP3 25
#define TMP4 26
#define TMP5 27
#define TMP6 28
#define TMP7 29
#define HI0 30
#define LOW0 31
#define ZERO 32
#define SP1 33
#define FP1 34
#define  GP1 35
#define  RA1 36
#define  SYSARG0 37
#define  SYSARG1 38
#define  SYSARG2 39
#define  SYSARG3 40
#define  SYSARG4 41
#define  SYSARG5 42
#define  SYSRVAL0 43
#define  SYSRVAL1 44
#define  SYSRVAL2 45
#define  SYSRVAL3 46
#define  SYSRVAL4 47
#define  GPR0 48
#define  GPR1 49
#define  GPR2 50
#define  GPR3 51
#define  GPR4 52
#define  GPR5 53
#define  GPR6 54
#define  GPR7 55
#define  SYSTMPR0 56
#define  SYSTMPR1	= 57;
#define  SYSTMPR2	= 58;
#define  SYSTMPR3	= 59;
#define  SYSTMPR4	= 60;
#define  SYSTMPR5	= 61;
#define  SYSREGHI0	= 62;
#define  SYSREGLOW0 	= 63;


#endif /* FUSION_CORE_SIM_H */
