#include "stdint.h"
#include "fusion.h"
#include "fusion-opc.h"
#include "emul_error.h"

#define TRUE (_Bool) 1
#define FALSE (_Bool) 0

/* Simulation Settings */
typedef struct {
	_Bool	pipeline;	/* Enabling pipline simulation */
	_Bool	oooe;		/* Enable Out of order Execution*/	
} sim_set;

/* Enumberation of Instruction Execution Processors */
typedef enum {
	CPID_MAIN,			/* Main instruction Set */
	CPID_SYS,			/* System instructions */
	CPID_END				/* Maximum */

} CPID_t;

/* Enumberation of CPU Pipeline Stages */
typedef enum {
	FETCH,
	DECODE,
	EXECUTE,
	MEM,
	WB,
	MAX_STAGES
} CPU_CYCLE;



/* Struct for unpacking Arithmetic type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rd		: 5;	/* Destination Register */
	uint8_t rsa		: 5;	/* Source Register A */
	uint8_t	rsb		: 5;	/* Source Register B */
	uint8_t empty	: 7;	/* Unused Space */
	uint8_t aluop	: 4;	/* ALU Operation */
} RInsn_t;

/* Struct for unpacking Immediate type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rd		: 5;	/* Destination Register */
	uint8_t rsa		: 5;	/* Source Register A */
	uint16_t imm	: 12;	/* 12 bit Immediate */
	uint8_t aluop	: 4;	/* ALU Operation */
} ImmInsn_t;

/* Struct for unpacking Load type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rd		: 5;	/* Destination Register */
	uint8_t rsa		: 5;	/* Source Register A */
	uint8_t funct	: 2;	/* Function Field */
	uint16_t imm	: 14;	/* 14 bit Immediate */
} LdInsn_t;

/* Struct for unpacking Load Immediate type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rd		: 5;	/* Destination Register */
	uint8_t dsel	: 5;	/* Source Register A */
	uint16_t imm	: 16;	/* 16 bit Immediate */
} LiInsn_t;

/* Struct for unpacking Store type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t funct	: 2;	/* Function Field */
	uint8_t rsa		: 5;	/* Source Register A */
	uint8_t rsb		: 5;	/* Source Register B */
	uint16_t imm	: 14;	/* 14 bit Immediate */
} StInsn_t;

/* Struct for unpacking Jump type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rsa		: 5;	/* Source Register A */
	uint32_t imm	: 21;	/* 21 bit Immediate */
	_Bool link;				/* Flag for linking address */
} JInsn_t;

/* Struct for unpacking Branch type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t rd		: 5;	/* Destination Register */
	uint8_t rsa		: 5;	/* Source Register A */
	uint8_t	rsb		: 5;	/* Source Register B */
	uint16_t imm	: 14;	/* 14 bit Immediate */
	uint8_t funct	: 2;	/* Function Field */
} BInsn_t;

/* Struct for unpacking System type Instructions */
typedef struct {
	uint8_t opcode 	: 6;	/* Opcode field */
	uint8_t	rd		: 5;	/* Destination Register */
	uint8_t rsa		: 5;	/* Source Register A */
	uint8_t imm		: 8;	/* 8 bit Immediate */
	uint8_t funct	: 8;	/* Function Field */
} SysInsn_t;

/* Generic Instruction Type */
typedef union{
	RInsn_t 	rtype;
	ImmInsn_t	itype;
	LdInsn_t	ltype;
	LiInsn_t	litype;
	StInsn_t	stype;
	JInsn_t		jtype;
	BInsn_t		btype;
	SysInsn_t	systype;
} insn_t;

/* Struct for defining instruction and accompanying data */
typedef struct {
	uint32_t	word;		/* Instruction word */
	CPID_t 		insn_type;	/* Co-Processor type */
	CPU_CYCLE 	cycle;		/* Current cycle in CPU */
	int			regf_d;		/* Register File for destination */
	int			regf_s;		/* Register File for source */
} insn_info;



/* Instructions in flight for pipelined machine */
insn_info insn_pipeline[ MAX_STAGES - 1 ]; 

/* Prototypes */

uint32_t fetch( uint32_t** imem, uint32_t pc );
int execute( insn_info* insn_i, uint32_t** registers, uint32_t* pc, uint32_t* cycleno, uint32_t** data_mem, dataseg_info dseg_i );
insn_t parse_insn( uint32_t word );


