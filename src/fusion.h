/* Definitions for decoding the Fusion-Core opcode table.
   Copyright (C) 2009-2017 Free Software Foundation, Inc.
   Contributed by Dylan Wadler.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef FUSION_OPC_H
#define FUSION_OPC_H

#include <stdint.h>

#define FUSION_LITTLE_ENDIAN 1	/* Use little endian for fusion-core */

/* Operand Type Definitions*/
#define USE_NONE			0	//Doesn't have use, just for system instructions
#define USE_RDAB			1	//Uses Rd, RSa, RSb
#define USE_RDAI			2	//Uses Rd, RSa, Immediate
#define USE_RDI				3	//Uses Rd, Immediate
#define USE_RABI			4	//Uses RSa, RSb, Immediate
#define USE_RAI				5	//Uses RSa, Immediate
#define USE_I				6	//Uses Immediate only
#define USE_RDA				7	//Uses Rd, RSa
#define USE_RAB				8	//Uses RSa, RSb
#define USE_RABI_O			9	//Uses RSa, RSb, immediate, with rsb
								//as offset value
#define USE_RDAI_O			10  //Same as USE_RDAI, but with RSa
								//as offset value
#define USE_RAI_O		11  //Same as USE_RAI, but with RSa
								//as offset value							//
#define MAX_USE_OP		12

/*Defines for number of current implemented instructions*/
#define NUM_INSN_R			17 // 16 total instructions,only 12 at
					   //the moment; include nop to 16 
					   //other instructions
					   
#define NUM_INSN_I			16 // 16 total instructions, only 12 a the moment
#define NUM_INSN_L			4
#define NUM_INSN_LI			32 //32 total instructions, only 6 at the moment
#define NUM_INSN_S			4
#define NUM_INSN_J			2
#define NUM_INSN_JL			2
#define NUM_INSN_B			4
#define NUM_INSN_SYS		256 //256 total instructions, only 7 at the moment
#define NUM_INSN		( NUM_INSN_R + NUM_INSN_I + NUM_INSN_L\
						+ NUM_INSN_LI + NUM_INSN_S + NUM_INSN_J\
						+ NUM_INSN_JL + NUM_INSN_SYS )

/**** Bit field Masks ****/
#if FUSION_LITTLE_ENDIAN 
/* Little endian defintions */

/* Bitfield offsets */
#define OFFSET_OPC			0
#define OFFSET_RD			6
#define OFFSET_RSA			11
#define OFFSET_RSB			16
#define OFFSET_SHFT			21
#define OFFSET_ALUOP		28
#define OFFSET_IMM_I		16
#define OFFSET_IMM_L		18
#define OFFSET_FUNCT_L		16
#define OFFSET_DSEL_LI		11
#define OFFSET_IMM_LI		16
#define OFFSET_FUNCT_S		23
#define OFFSET_IMM_HI_S		21
#define OFFSET_IMM_LO_S		8
#define OFFSET_IMM_HI_J		OFFSET_RD
#define OFFSET_IMM_LO_J		16
#define OFFSET_IMM_HI_B		21
#define OFFSET_IMM_LO_B		6
#define OFFSET_FUNCT_B		30
#define OFFSET_FUNCT_SYS	16
#define OFFSET_IMM_SYS		24

/* Bit masks for non-shifted bitfields */
#define MASK_2bit			(unsigned)0x00000003
#define MASK_3bit			(unsigned)0x00000007
#define MASK_4bit			(unsigned)0x0000000f
#define MASK_5bit			(unsigned)0x0000001f
#define MASK_6bit			(unsigned)0x0000003f
#define MASK_7bit			(unsigned)0x0000007f
#define MASK_8bit			(unsigned)0x000000ff
#define MASK_11bit			(unsigned)0x000007ff
#define MASK_12bit			(unsigned)0x00000fff
#define MASK_14bit			(unsigned)0x00003fff
#define MASK_16bit			(unsigned)0x0000ffff
#define MASK_21bit			(unsigned)0x001fffff

#define MASK_RD				(MASK_5bit << OFFSET_RD)//0x000003e0
#define MASK_RSA			(MASK_5bit << OFFSET_RSA)//0x00007c00
#define MASK_RSB			(MASK_5bit << OFFSET_RSB)//0x0001f000
#define MASK_SHFT			(MASK_7bit << OFFSET_SHFT)//0x000007f0	
#define MASK_ALUOP			(MASK_4bit << OFFSET_ALUOP)//0xf0000000
#define MASK_IMM_I			(MASK_12bit << OFFSET_IMM_I)//0x0fff0000
#define MASK_IMM_L			(MASK_14bit << OFFSET_IMM_L)//0xfff80000
#define MASK_FUNCT_L		(MASK_2bit << OFFSET_FUNCT_L)//0x00006000
#define MASK_DSEL_LI		MASK_RSA
#define MASK_IMM_LI			(MASK_16bit << OFFSET_IMM_LI)//0xffff0000
#define MASK_FUNCT_S		MASK_RSB
#define MASK_IMM_HI_S		(MASK_11bit << OFFSET_IMM_HI_S)//0xfff00000
#define MASK_IMM_LO_S		(MASK_3bit << OFFSET_IMM_LO_S)//0x00000700
#define MASK_IMM_S			( MASK_IMM_LO_S | MASK_IMM_HI_S ) //0xfff00700
#define MASK_IMM_HI_J		MASK_RD//0xffff0000	
#define MASK_IMM_LO_J		(MASK_16bit << OFFSET_IMM_LO_J)
#define MASK_IMM_J			( (MASK_IMM_LO_J) | (MASK_IMM_HI_J) )//0xffff03e0
#define MASK_IMM_HI_B		(MASK_11bit << OFFSET_IMM_HI_B)//0x03fe0000
#define MASK_IMM_LO_B		MASK_RD
#define MASK_IMM_B			( MASK_IMM_LO_B | MASK_IMM_HI_B )//0x03fe03e0
#define MASK_FUNCT_B		( MASK_2bit << OFFSET_FUNCT_B )//0xc0000000
#define MASK_FUNCT_SYS		( MASK_8bit << OFFSET_FUNCT_SYS)//0x0f000000
#define MASK_IMM_SYS		( MASK_8bit << OFFSET_IMM_SYS)//0xf0000000

/*Opcode related defines*/
#define MASK_OPC			MASK_6bit
#define MASK_R_OPC			OPC_INT
#define MASK_LI_OPC			OPC_LI
#define MASK_IMM_OPC		OPC_IMM
#define MASK_B_OPC			OPC_BRANCH
#define MASK_JMP_OPC		OPC_JMP
#define MASK_JLNK_OPC		OPC_JLNK
#define MASK_RMEM_OPC		OPC_LD
#define MASK_WMEM_OPC		OPC_ST
#define MASK_SYS_OPC		OPC_SYS //more system instructions may change this
#define MASK_MSB_OPC		0x00000011	//31st bit of instruction
#define MASK_NO_IMM			0x00000000





/* Bit field split mask for immediates*/
#define SPLIT_S_IMM_LO		(MASK_IMM_HI_S >> OFFSET_IMM_HI_S) 
#define SPLIT_S_IMM_HI		(MASK_IMM_LO_S >> OFFSET_IMM_LO_S)
#define SPLIT_J_IMM_LO		(MASK_IMM_HI_J >> OFFSET_IMM_HI_J) 
#define SPLIT_J_IMM_HI		(MASK_IMM_LO_J >> OFFSET_IMM_LO_J)
#define SPLIT_B_IMM_LO		(MASK_IMM_HI_B >> OFFSET_IMM_HI_B) 
#define SPLIT_B_IMM_HI		(MASK_IMM_LO_B >> OFFSET_IMM_LO_B)

#else
/* Big endian definitions */
#define MASK_RD				0x03e00000
#define MASK_RSA			0x001f0000
#define MASK_RSB			0x0000f800
#define MASK_SHFT			0x000007f0	
#define MASK_ALUOP			0x0000000f
#define MASK_IMM_I			0x0000fff0
#define MASK_IMM_L			0x00003fff
#define MASK_FUNCT_L		0x0000c000
#define MASK_DSEL_LI		0x001e0000
#define MASK_IMM_LI			0x0000ffff
#define MASK_FUNCT_S		0x0f000000
#define MASK_IMM_S			0x00e007ff
#define MASK_IMM_HI_S		0x00e00000
#define MASK_IMM_LO_S		0x000007ff
#define MASK_IMM_J			0x03e0ffff
#define MASK_IMM_HI_J		0x03e00000	
#define MASK_IMM_LO_J		0x0000ffff
#define MASK_IMM_B			0x03e007fc
#define MASK_IMM_HI_B		0x03e00000
#define MASK_IMM_LO_B		0x000007fc
#define MASK_FUNCT_B		0x00000003
#define MASK_FUNCT_SYS		0x000000f0
#define MASK_IMM_SYS		0x0000000f

/* Bit field split mask for immediates*/
#define SPLIT_S_IMM_LO		0x000007ff //lower byte
#define SPLIT_S_IMM_HI		0x00003800 
#define SPLIT_J_IMM_LO		0x0000ffff //lower 16 bits
#define SPLIT_J_IMM_HI		0x001f0000
#define SPLIT_B_IMM_LO		0x000001ff //lower byte
#define SPLIT_B_IMM_HI		0x00003e00 

/* Bitfield offsets */
#define OFFSET_OPC			26
#define OFFSET_RD			21
#define OFFSET_RSA			16
#define OFFSET_RSB			11
#define OFFSET_SHFT			4
#define OFFSET_ALUOP		0
#define OFFSET_IMM_I		4
#define OFFSET_IMM_L		0
#define OFFSET_FUNCT_L		14
#define OFFSET_DSEL_LI		17
#define OFFSET_IMM_LI		0
#define OFFSET_FUNCT_S		23
//#define SHFT_IMM_HI_S		10 //11 -> 21
//#define SHFT_IMM_HI_J		5 //16 ->21
//#define SHFT_IMM_HI_B		12 //9->21
//#define SHFT_IMM_LO_B		2 //0 -> 2
#define OFFSET_IMM_HI_S		10
#define OFFSET_IMM_LO_S		5
#define OFFSET_IMM_HI_B		12
#define OFFSET_IMM_LO_B		2
#define OFFSET_IMM_HI_J		5
#define OFFSET_IMM_LO_J		0
#define OFFSET_FUNCT_B		0
#define OFFSET_FUNCT_SYS	8
#define OFFSET_IMM_SYS		0

#define MASK_OPC			0xfc000000 //upper 6 bits gives opcode
#define MASK_R_OPC			OPC_INT
#define MASK_LI_OPC			OPC_LI
#define MASK_IMM_OPC		OPC_IMM
#define MASK_B_OPC			OPC_BRANCH
#define MASK_JMP_OPC		OPC_JMP
#define MASK_JLNK_OPC		OPC_JLNK
#define MASK_RMEM_OPC		OPC_LD
#define MASK_WMEM_OPC		OPC_ST
#define MASK_SYS_OPC		OPC_SYS //more system instructions may change this
#define MASK_MSB_OPC		0x11000000	//31st bit of instruction
#define MASK_NO_IMM			0x00000000

#endif

/*OPCode match macros*/
#define IS_CP_INSN(i)	(GET_OPC(i) == MASK_MSB_OPC) 	//is coprocessor instruction if first bit of opcode is 1
#define IS_R_INSN(i)	(GET_OPC(i) == MASK_R_OPC)  			//returns 1 if match, otherwise 0
#define IS_LI_INSN(i)	(GET_OPC(i) == MASK_LI_OPC)
#define IS_IMM_INSN(i)	(GET_OPC(i) == MASK_IMM_OPC)
#define IS_B_INSN(i)	(GET_OPC(i) == MASK_B_OPC)
#define IS_JMP_INSN(i)	(GET_OPC(i) == MASK_JMP_OPC)
#define IS_JLNK_INSN(i)	(GET_OPC(i) == MASK_JLNK_OPC)
#define IS_RMEM_INSN(i)	(GET_OPC(i) == MASK_RMEM_OPC)
#define IS_WMEM_INSN(i)	(GET_OPC(i) == MASK_WMEM_OPC)
#define IS_SYS_INSN(i)	(GET_OPC(i) == MASK_SYS_OPC)

/*Determining format Macros*/
#define IS_R_TYPE(i) 	( IS_R_INSN(i) )	
#define IS_I_TYPE(i) 	( IS_IMM_INSN(i) ) 
#define IS_L_TYPE(i) 	( IS_RMEM_INSN(i)  )
#define IS_LI_TYPE(i) 	( IS_LI_INSN(i) )
#define IS_S_TYPE(i) 	( IS_WMEM_INSN(i) )
#define IS_J_TYPE(i) 	( IS_JMP_INSN(i) )
#define IS_JL_TYPE(i)   ( IS_JLNK_INSN(i) )
#define IS_B_TYPE(i) 	( IS_B_INSN(i) )
#define IS_SYS_TYPE(i) 	( IS_SYS_INSN(i) )


/*Defines for Instruction formats*/
#define FRMT_R				0
#define FRMT_I				1
#define FRMT_L				2
#define FRMT_LI				3
#define FRMT_S				4
#define FRMT_J				5
#define FRMT_B				6
#define FRMT_SYS			7
#define FRMT_MAX			9

/*Defines for Co-Processor ID*/
//#define CPID_MAIN		0
//#define CPID_MACRO		1
//#define CPID_MAX		2

/*Defines for Opcodes*/
#define OPC_INT			0x13 //010011
#define OPC_IMM			0x16 //010110
#define OPC_LD			0x1e //011110
#define OPC_LI			0x10 //010000
#define OPC_ST			0x1d //011101
#define OPC_JMP			0x0c //001100	
#define OPC_JLNK		0x04 //000100
#define OPC_BRANCH		0x0d //001101
#define OPC_SYS			0x20 //100000


/* ABI names for selected x-registers
 *
 * helpful for debug information */

#define X_RA 4 	/* Return address register */
#define X_SP 1 	/* Stack pointer register */
#define X_GP 3	/* Global pointer register */
#define X_T0 22 /* Temporary register names */
#define X_T1 23
#define X_T2 24
#define X_T3 25
#define X_T4 26
#define X_T5 27
#define X_T6 28
#define X_T7 29
#define NGPR 32	/* Number of general purpose registers */
#define NFPR 32 /* Number of floating point registers */


/*Instruction bits*/

/*Macro IDs*/
enum{
	M_LA,
	M_CALL,
	M_RET,
	M_MAX

};

#endif
