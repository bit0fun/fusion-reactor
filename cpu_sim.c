#include "cpu_sim.h"

/* Simulation of Fetch stage, returns instruction */
uint32_t fetch( uint32_t** imem, uint32_t pc ){
	return (*imem)[ pc/4 ];
}

/* Simulation execution of instructions for pure functionality
 * of the ISA. no additional features are to be used here */
int execute( insn_info* insn_i, uint32_t (*registers)[32], uint32_t* pc, uint32_t* cycleno, uint32_t** data_mem, dataseg_info dseg_i ){
	uint32_t insn_word = insn_i->word;
	uint32_t* gpregf = (*registers);
	insn_t insn_parts = parse_insn( insn_word );
	uint8_t opcode = GET_OPC( insn_word );
	int rd;
	uint32_t rsa;
	uint32_t rsb;
	uint16_t imm;
	switch ( opcode ){
		/* Integer \ Arithmetic Instructions */
		case OPC_INT :
			rd  = insn_parts.rtype.rd;
			rsa = (*registers)[insn_parts.rtype.rsa];
			rsb = (*registers)[insn_parts.rtype.rsb];

			/* Determine ALUOP */
			switch ( insn_parts.rtype.aluop ) {
				case 0 :
					(*registers)[rd] = add( rsa, rsb );
					break;
				case  1 :
					(*registers)[rd] = sub( rsa, rsb );
					break;
				case  4 :
					(*registers)[rd] = _not( rsa, rsb );
					break;
				case  5 :
 					(*registers)[rd] = _and( rsa, rsb );
					break;
				case  6 :
 					(*registers)[rd] = _or( rsa, rsb );
					break;
				case  7 :
 					(*registers)[rd] = _xor( rsa, rsb );
					break;
				case  8 :
 					(*registers)[rd] = sal( rsa, rsb );
					break;
				case  9 :
 					(*registers)[rd] = sar( rsa, rsb );
					break;
				case  10 :
 					(*registers)[rd] = sll( rsa, rsb );
					break;
				case  11 :
 					(*registers)[rd] = slr( rsa, rsb );
					break;
				case  12 :
 					(*registers)[rd] = comp( rsa, rsb );
					break;
				default :
					/* Return error of incorrect Arithmetic instruction */
					*pc += 4;	/* Increment PC */
					return EM_INT;
					break;
			}
			*pc += 4;	/* Increment PC */
			break;
		case OPC_IMM :
			imm = insn_parts.itype.imm;
			rd	= insn_parts.itype.rd;
			rsa	= (*registers)[insn_parts.itype.rsa];

			/* Determine ALUOP */
			switch ( insn_parts.rtype.aluop ) {
				case 0 :
					(*registers)[rd] = addi( rsa, imm );
					break;
				case  1 :
					(*registers)[rd] = subi( rsa, imm );
					break;
				case  4 :
					(*registers)[rd] = noti( imm );
					break;
				case  5 :
 					(*registers)[rd] = andi( rsa, imm );
					break;
				case  6 :
 					(*registers)[rd] = ori( rsa, imm );
					break;
				case  7 :
 					(*registers)[rd] = xori( rsa, imm );
					break;
				case  8 :
 					(*registers)[rd] = sali( rsa, imm );
					break;
				case  9 :
 					(*registers)[rd] = sari( rsa, imm );
					break;
				case  10 :
 					(*registers)[rd] = slli( rsa, imm );
					break;
				case  11 :
 					(*registers)[rd] = slri( rsa, imm );
					break;
				case  12 :
 					(*registers)[rd] = compi( rsa, imm );
					break;
				default :
					*pc += 4;	/* Increment PC */
					return EM_IMM;
					break;
			}
			*pc += 4;	/* Increment PC */
			break;
		case OPC_LD :
			rd	= insn_parts.ltype.rd;
			rsa	= (*registers)[insn_parts.ltype.rsa];
			imm	= insn_parts.ltype.rd;

			/* Finding size of data to read from memory */
			switch( insn_parts.ltype.funct ){
				/* Word */
				case 0 :
					(*registers)[rd] = lw(data_mem, dseg_i, rsa, imm);
					break;
				/* Half Word ( upper ) */
				case 1 :
					(*registers)[rd] = lh(data_mem, dseg_i, rsa, imm);
					break;
				/* 3 Quarter Word ( upper ) */
				case 2 :
					(*registers)[rd] = lth(data_mem, dseg_i, rsa, imm);
					break;
				/* Byte */
				case 3 :
					(*registers)[rd] = lb(data_mem, dseg_i, rsa, imm);
					break;
				default :
					*pc += 4;	/* Increment PC */
					return EM_LD;
					break;
			}
			*pc += 4;	/* Increment PC */
			break;
		case OPC_LI :
			rd	= insn_parts.litype.rd;
			imm = insn_parts.litype.imm;

			switch( insn_parts.litype.dsel ) {
				case 0 :
					(*registers)[rd] = li( imm );
					break;
				case 1 :
					(*registers)[rd] = lsi( imm );
					break;
				case 2 :
					(*registers)[rd] = lgi( imm );
					break;
				case 4 :
					(*registers)[rd] = lui( imm );
					break;
				case 5 :
					(*registers)[rd] = lusi( imm );
					break;
				case 6 :
					(*registers)[rd] = lugi( imm );
					break;
				case 8 :
					(*registers)[rd] = lni( imm );
					break;
				case 9 :
					(*registers)[rd] = lnsi( imm );
					break;
				case 10 :
					(*registers)[rd] = lngi( imm );
					break;
				case 12 :
					(*registers)[rd] = luni( imm );
					break;
				case 13 :
					(*registers)[rd] = lunsi( imm );
					break;
				case 14 :
					(*registers)[rd] = lungi( imm );
					break;
				default :
					*pc += 4;	/* Increment PC */
					return EM_LI;
					break;
			}
			*pc += 4;	/* Increment PC */
			break;
		case OPC_ST :
			rsa = (*registers)[insn_parts.stype.rsa];
			rsb = (*registers)[insn_parts.stype.rsb];
			imm = insn_parts.stype.imm;
			/* Finding size of data to write to memory */
			switch( insn_parts.ltype.funct ){
				/* Word */
				case 0 :
					sw(data_mem, dseg_i, rsa, rsb, imm);
					break;
				/* Half Word ( upper ) */
				case 1 :
					sh(data_mem, dseg_i, rsa, rsb, imm);
					break;
				/* 3 Quarter Word ( upper ) */
				case 2 :
					sth(data_mem, dseg_i, rsa, rsb, imm);
					break;
				/* Byte */
				case 3 :
					sb(data_mem, dseg_i, rsa, rsb, imm);
					break;
				default :
					*pc += 4;	/* Increment PC */
					return EM_ST;
					break;
			}
			*pc += 4;	/* Increment PC */
			break;
		case OPC_JMP :
			/* Default to Jump Register, and if RSA = 0, function will alter it
			 * to normal Jump */
			jr( pc, (*registers)[insn_parts.jtype.rsa], insn_parts.jtype.imm );
			break;
		case OPC_JLNK :
			/* Default to Jump Register and Link, and if RSA = 0, function will alter it
			 * to normal Jump and Link */
			jrl( pc, (*registers)[insn_parts.jtype.rsa], insn_parts.jtype.imm, (registers)[4] );
			break;
		case OPC_BRANCH :
			/* Can sample branch taken, at the moment this is not used.
			 * Can be helpful for debugging ideas for branch predictors or
			 * performance with branches */
			rsa = (*registers)[ insn_parts.btype.rsa];
			rsb = (*registers)[ insn_parts.btype.rsb];
			imm = insn_parts.btype.imm;
			/* Determining Branch type */
			switch( insn_parts.btype.funct ) {
				case 0 :
					beq( pc, rsa, rsb, imm);
					break;
				case 1 :
					bne( pc, rsa, rsb, imm);
					break;
				case 2 :
					bgt( pc, rsa, rsb, imm);
					break;
				case 3 :
					blt( pc, rsa, rsb, imm);
					break;
				default :
					return EM_B;
			}
			break;
		case OPC_SYS :
			/* No system instructions yet, so error */
			*pc += 4;	/* Increment PC */
			return EM_SYS;
			break;
		default :
			/* Must be co-processor instruction, return error */
			*pc += 4;	/* Increment PC */
			return EM_CP;
			break;

	}
}

/* Parses instruction, finds the type and returns the unionized struct
 * of the proper instruction format */
insn_t parse_insn( uint32_t word ){
	insn_t insn; /* Creation of the instruction type */

	if( IS_R_TYPE(word) ) {	//need to get alu op to determine instruction
		insn.rtype.opcode 	= GET_OPC(word);
		insn.rtype.rd 		= GET_RD(word);
		insn.rtype.rsa 		= GET_RSA(word);
		insn.rtype.rsb 		= GET_RSB(word);
		insn.rtype.aluop 	= GET_ALUOP(word);
	} else if( IS_I_TYPE(word) ) { //need to get alu op to determine instruction
		insn.itype.opcode 	= GET_OPC(word);
		insn.itype.rd 		= GET_RD(word);

		insn.itype.rsa		= GET_RSA(word);
		insn.itype.imm		= GET_IMM_I(word);
		insn.itype.aluop	= GET_ALUOP(word);
	} else if( IS_L_TYPE(word) ) {
		insn.ltype.opcode	= GET_OPC(word);
		insn.ltype.rd		= GET_RD(word);
		insn.ltype.rsa		= GET_RSA(word);
		insn.ltype.imm		= GET_IMM_L(word);
		insn.ltype.funct	= GET_FUNCT_L(word);
	} else if( IS_LI_TYPE(word) ) {
		insn.litype.opcode	= GET_OPC(word);
		insn.litype.rd		= GET_RD(word);
		insn.litype.dsel	= GET_DSEL_LI(word);
		insn.litype.imm		= GET_IMM_LI(word);
	} else if( IS_S_TYPE(word) ) {
		insn.stype.opcode	= GET_OPC(word);
		insn.stype.rsa		= GET_RSA(word);
		insn.stype.rsb		= GET_RSB(word);
		insn.stype.funct	= GET_FUNCT_S(word);
	} else if( IS_J_TYPE(word) ) {
		insn.jtype.opcode 	= GET_OPC(word);
		insn.jtype.rsa		= GET_RSA(word);
		insn.jtype.imm		= GET_IMM_J(word);
		/* Determining if linking PC or not */
		if( IS_JMP_INSN(word) )
			insn.jtype.link	= FALSE;
		else if ( IS_JLNK_INSN(word) )
			insn.jtype.link = TRUE;
		else {	/* Something wrong, create nop */
			insn.jtype.opcode = 0; /* zero in opcode will always be nop */
		}

	} else if( IS_B_TYPE(word) ) {
		insn.btype.opcode	= GET_OPC(word);
		insn.btype.rsa		= GET_RSA(word);
		insn.btype.rsb		= GET_RSB(word);
		insn.btype.imm		= GET_IMM_B(word);
		insn.btype.funct	= GET_FUNCT_B(word);
	} else if( IS_SYS_TYPE(word) ) {
		insn.systype.opcode	= GET_OPC(word);
		insn.systype.rd		= GET_RD(word);
		insn.systype.rsa	= GET_RSA(word);
		insn.systype.imm	= GET_IMM_SYS(word);
		insn.systype.funct	= GET_FUNCT_SYS(word);
	} else if( IS_CP_INSN(word) ) {
		/* TO-DO Add in co-processor support */

		/* For now, just create nop */
		insn.rtype.opcode = 0;
	} else {
		/* Anything else is just nop, for error handling */
		insn.rtype.opcode = 0;
	}

	return insn;
}
