#include "cpu_sim.h"

/* Simulation of Fetch stage, returns instruction */
uint32_t fetch( uint32_t** imem, uint32_t pc ){
	return (*imem)[ pc/4 ];
}

/* Simulation execution of instructions for pure functionality
 * of the ISA. no additional features are to be used here */
int execute( insn_info* insn_i, uint32_t** registers, uint32_t* pc, uint32_t* cycleno, uint32_t** data_mem, dataseg_info dseg_i ){	
	uint32_t insn_word = insn_i->word;
	uint32_t* gpregf = (*registers);
	insn_t insn_parts = parse_insn( insn_word );	
	uint8_t opcode = GET_OPC( insn_word );	
	switch ( opcode ){
		/* Integer \ Arithmetic Instructions */
		case OPC_INT :
			int		 rd  = insn_parts.rtype.rd;
			uint32_t rsa = (*registers)[insn_parts.rtype.rsa];
			uint32_t rsb = (*registers)[insn_parts.rtype.rsb];
			
			/* Determine ALUOP */
			switch ( insn_parts.rtype.aluop ) {
				case 0 :
					(*registers)[rd] = add( rsa, rsb );	
					break;
				case  1 :
					(*registers)[rd] = sub( rsa, rsb );
					break;
				case  4 :
					(*registers)[rd] = not( rsa, rsb );
					break;	   
				case  5 :
 					(*registers)[rd] = and( rsa, rsb );
					break;
				case  6 :
 					(*registers)[rd] = or( rsa, rsb );
					break;
				case  7 :
 					(*registers)[rd] = xor( rsa, rsb );
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
					return 
					break;
			}
			break;
		case OPC_IMM :
			int 		rd 	= insn_parts.itype.rd;
			uint32_t 	rsa	= (*registers)[insn_parts.itype.rsa];
			uint16_t 	imm	= (*registers)[insn_parts.itype.rd];
			/* Determine ALUOP */
			switch ( insn_parts.rtype.aluop ) {
				case 0 :
					(*registers)[rd] = addi( rsa, imm );	
					break;
				case  1 :
					(*registers)[rd] = subi( rsa, imm );
					break;
				case  4 :
					(*registers)[rd] = noti( rsa, imm );
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
					/* default to NOP */
					break;
			}
			break;
		case OPC_LD :
			int 		rd	= insn_parts.ltype.rd;
			uint32_t 	rsa	= (*registers)[insn_parts.ltype.rsa];
			uint16_t 	imm	= (*registers)[insn_parts.ltype.rd];

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
					break;
			}
			break;
		case OPC_LI :
			int 		rd	= insn_parts.litype.rd;
			uint16_t 	imm = insn_parts.litype.imm;

			switch( insn_parts.litype.dsel ) {
				case 0 :
					(*registers)li( imm );
					break;
				case 1 :
					(*registers)lsi( imm );
					break;
				case 2 :
					(*registers)lgi( imm );
					break;
				case 4 :
					(*registers)lui( imm );
					break;
				case 5 :
					(*registers)lusi( imm );
					break;
				case 6 :
					(*registers)lugi( imm );
					break;
				case 8 :
					(*registers)lni( imm );
					break;
				case 9 :
					(*registers)lnsi( imm );
					break;
				case 10 :
					(*registers)lngi( imm );
					break;
				case 12 :
					(*registers)luni( imm );
					break;
				case 13 :
					(*registers)lunsi( imm );
					break;
				case 14 :
					(*registers)lungi( imm );
					break;
				default :
					break;
			}
			break;
		case OPC_ST :
			uint32_t rsa = (*registers)[insn_parts.stype.rsa];
			uint32_t rsb = (*registers)[insn_parts.stype.rsb];
			uint16_t imm = insn_parts.stype.imm;
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
					break;
			}
			break;
		case OPC_JMP :
			/* Default to Jump Register, and if RSA = 0, function will alter it
			 * to normal Jump */
			jr( pc, (*registers)[insn_parts.jtype.rsa], insn_parts.jtype.imm );
			break;
		case OPC_JLNK :
			/* Default to Jump Register and Link, and if RSA = 0, function will alter it
			 * to normal Jump and Link */
			jrl( pc, (*registers)[insn_parts.jtype.rsa], insn_parts.jtype.imm );
			break;
		case OPC_BRANCH :
			/* Can sample branch taken, at the moment this is not used.
			 * Can be helpful for debugging ideas for branch predictors or 
			 * performance with branches */
			uint32_t rsa = (*registers)[ insn_parts.btype.rsa];
			uint32_t rsb = (*registers)[ insn_parts.btype.rsb];
			uint16_t imm = insn_parts.btype.imm;
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
			}
			break;
		case OPC_SYS :
			break;
		default :
			return -1;
	
	}
}

/* Parses instruction, finds the type and returns the unionized struct
 * of the proper instruction format */
insn_t parse_insn( uint32_t word ){
	insn_t insn; /* Creation of the instruction type */

	if( IS_R_TYPE(word) ) {	//need to get alu op to determine instruction
		insn.rtype.opcode 	= (uint8_t)GET_OPC(word);
		insn.rtype.rd 		= (uint8_t)GET_RD(word);
		insn.rtype.rsa 		= (uint8_t)GET_RSA(word);
		insn.rtype.rsb 		= (uint8_t)GET_RSB(word);
		insn.rtype.aluop 	= (uint8_t)GET_ALUOP(word);
	} else if( IS_I_TYPE(word) ) { //need to get alu op to determine instruction
		insn.itype.opcode 	= (uint8_t)GET_OPC(word);
		insn.itype.rd 		= (uint8_t)GET_RD(word);
		insn.itype.rsa		= (uint8_t)GET_RSA(word);
		insn.itype.imm		= (uint16_t)GET_IMM_I(word);
		insn.itype.aluop	= (uint8_t)GET_ALUOP(word);
	} else if( IS_L_TYPE(word) ) {
		insn.ltype.opcode	= (uint8_t)GET_OPC(word);
		insn.ltype.rd		= (uint8_t)GET_RD(word);
		insn.ltype.rsa		= (uint8_t)GET_RSA(word);
		insn.ltype.imm		= (uint16_t)GET_IMM_L(word);
		insn.ltype.funct	= (uint8_t)GET_FUNCT_L(word);
	} else if( IS_LI_TYPE(word) ) {
		insn.litype.opcode	= (uint8_t)GET_OPC(word);
		insn.litype.rd		= (uint8_t)GET_RD(word);
		insn.litype.dsel	= (uint8_t)GET_DSEL_LI(word);
		insn.litype.imm		= (uint16_t)GET_IMM_LI(word);
	} else if( IS_S_TYPE(word) ) {
		insn.stype.opcode	= (uint8_t)GET_OPC(word);
		insn.stype.rsa		= (uint8_t)GET_RSA(word);
		insn.stype.rsb		= (uint8_t)GET_RSB(word);
		insn.stype.funct	= (uint8_t)GET_FUNCT_S(word);
	} else if( IS_J_TYPE(word) ) {
		insn.jtype.opcode 	= (uint8_t)GET_OPC(word);
		insn.jtype.rsa		= (uint8_t)GET_RSA(word);
		insn.jtype.imm		= (uint32_t)GET_IMM_J(word);
		/* Determining if linking PC or not */
		if( IS_JMP_INSN(word) )
			insn.jtype.link	= FALSE;
		else if ( IS_JLNK_INSN(word) )
			insn.jtype.link = TRUE;
		else {	/* Something wrong, create nop */
			insn.jtype.opcode = (uint8_t) 0; /* zero in opcode will always be nop */
		}
	
	} else if( IS_B_TYPE(word) ) {
		insn.btype.opcode	= (uint8_t)GET_OPC(word);
		insn.btype.rsa		= (uint8_t)GET_RSA(word);
		insn.btype.rsb		= (uint8_t)GET_RSB(word);
		insn.btype.imm		= (uint16_t)GET_IMM_B(word);
		insn.btype.funct	= (uint8_t)GET_FUNCT_B(word);
	} else if( IS_SYS_TYPE(word) ) {
		insn.systype.opcode	= (uint8_t)GET_OPC(word);	
		insn.systype.rd		= (uint8_t)GET_RD(word);
		insn.systype.rsa	= (uint8_t)GET_RSA(word);
		insn.systype.imm	= (uint8_t)GET_IMM_SYS(word);
		insn.systype.funct	= (uint8_t)GET_FUNCT_SYS(word);
	} else if( IS_CP_INSN(word) ) {
		/* TO-DO Add in co-processor support */

		/* For now, just create nop */
		insn.rtype.opcode = (uint8_t)0;
	} else {
		/* Anything else is just nop, for error handling */
		insn.rtype.opcode = (uint8_t)0;
	}

	return insn;
}
