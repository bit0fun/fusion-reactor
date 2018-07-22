#ifndef EMUL_ERROR_H
#define EMUL_ERROR_H

/* Defines for emulation error codes */
enum {
	EM_OK,			/* No Error */
	EM_INT,			/* Invalid Arithmetic Instruction */
	EM_IMM,			/* Invalid Immediate Instruction */
	EM_LD,			/* Invalid Load Instruction */
	EM_ST,			/* Invalid Store Instruction */
	EM_LI,			/* Invalid Load Immediate Instruction */
	EM_J,			/* Invalid Jump Instruction */
	EM_JL,			/* Invalid Jump and Link Instruction */
	EM_B,			/* Invalid Branch Instruction */
	EM_SYS,			/* Invalid System Instruction */
	EM_CP,			/* Invalid Co-Processor Instruction */
	EM_NCPID,		/* Invalid Co-Processor ID */
	EM_MAX			/* Maximum Number of Error Codes */
};

#endif /* EMUL_ERROR_H */
