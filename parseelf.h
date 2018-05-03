#ifndef PARSEELF_H
#define PARSEELF_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "fusion-opc.h"
#include "fusion.h"

typedef uint16_t Elf32_Half; /* Unsigned half int */
typedef uint32_t Elf32_Off;  /* Unisgned offset */
typedef uint32_t Elf32_Addr; /* Unsigned address */
typedef uint32_t Elf32_Word; /* Unsigned int */
typedef int32_t  Elf32_Sword; /* Signed int */




/* ELF Header Data Types*/

#define ELF_NIDENT 16

/* ELF Header struct */
typedef struct {
	uint8_t		e_ident[ELF_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word 	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off 	e_shoff;
	Elf32_Word 	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrnidx;
} Elf32_Ehdr;

enum Elf_Ident {
	EI_MAG0			= 0, /* 0x7F */ 
	EI_MAG1			= 1, /* 'E' */
	EI_MAG2			= 2, /* 'L' */
	EI_MAG3			= 3, /* 'F' */
	EI_CLASS		= 4, /* Architecture */
	EI_DATA			= 5, /* Byte order, which should be big endian in this case */
	EI_VERSION		= 6, /* ELF Version */
	EI_OSABI		= 7, /* OS Specific */
	EI_ABIVERSION 	= 8, /* OS Specific again*/
	EI_PAD			= 9, /* Padding used*/
};

#define ELFMAG0	0x7F	/* e_ident[EI_MAG0]*/
#define ELFMAG1	'E'		/* e_ident[EI_MAG1]*/
#define ELFMAG2	'L'		/* e_ident[EI_MAG2]*/
#define ELFMAG3	'F'		/* e_ident[EI_MAG3]*/

#define ELFDATA2LSB		1	/* Defined for error checking, but not used otherwise*/
#define ELFDATA2MSB		2	/* Big Endian for the Fusion-Core ISA*/

#define ELFCLASS32		1	/* 32 Bit Architecture*/

enum Elf_Type{
	ET_NONE		= 0,	/* Unknown Type */
	ET_REL		= 1, 	/* Relocatable File */
	ET_EXEC		= 2,	/* Executable File */
	ET_DYN		= 3,	/* Shared Object File */
};

#define EM_FUSION	0x1102	/* Machine type*/
#define EV_CURRENT	1		/* ELF Current Version */


/*ELF Section Header Data Types*/

/* Struct for Elf Section Header*/
typedef struct{
	Elf32_Word 		sh_name;
	Elf32_Word 		sh_type;
	Elf32_Word 		sh_flags;
	Elf32_Addr		sh_addr;
	Elf32_Off 		sh_offset;
	Elf32_Word 		sh_size;
	Elf32_Word 		sh_link;
	Elf32_Word 		sh_info;
	Elf32_Word 		sh_addralign;
	Elf32_Word 		sh_entsize;
} Elf32_Shdr;

#define SHN_UNDEF		(0x00) /* Undefined/Not present */
#define SHN_ABS			(0xfff1) /* Absolute Address */
#define SHN_COMMON		(0xfff2) /*Common or unallocated C extern variables*/

/* Types of sections*/
enum ShT_Types {
	SHT_NULL			= 0,	/* Null Section */
	SHT_PROGBITS		= 1, 	/* Program Information */
	SHT_SYMTBL			= 2, 	/* Symbol Table */
	SHT_STRTBL			= 3,	/* String Table */
	SHT_RELA			= 4,	/* Relocation using addend */
	SHT_NOBITS			= 8,	/* Not present in file */
	SHT_REL 			= 9,	/* Relcation without addend */
};

/* Section Attributes */
enum ShT_Attr {
	SHF_WRITE		= 0x01,	/* Writable section*/
	SHF_ALLOC		= 0x02,	/* Exists in memory, allocatable*/
	SHF_EXECINSTR	= 0x04,	/* Executable instructions */
};



/* Elf Section Data Types  */

/* Symbol Table Entry Struct */
typedef struct {
	Elf32_Word		st_name;
	Elf32_Addr		st_value;
	Elf32_Word		st_size;
	uint8_t			st_info;
	uint8_t			st_other;
	Elf32_Half		st_shndx;
} Elf32_Sym;

#define ELF32_ST_BIND(INFO)		((INFO) >> 4)
#define ELF32_ST_TYPE(INFO)		((INFO) & 0x0F)

enum StT_Bindings{
	STB_LOCAL		= 0, //Local Storage
	STB_GLOBAL		= 1, //Global Storage
	STB_WEAK		= 2  //Weak like when using __attribute__((weak)) 
};

/* Definitions of different Symbol Table Types */
enum StT_Types {
	STT_NOTYPE		= 0, //No type
	STT_OBJECT		= 1, //Variables, arrays, other data types
	STT_FUNC		= 2  //Functions
};


/* Relocation Data Types */

typedef struct{
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
} Elf32_Rel;

typedef struct{
	Elf32_Addr		r_offset;
	Elf32_Word		r_info;
	Elf32_Sword		r_addend;
} Elf32_Rela;

/* For getting info on relocation symbol and type */
#define ELF32_R_SYM(INFO)	((INFO) >> 8)
#define ELF32_R_TYPE(INFO)	((uint8_t) (INFO))

/* Relocation Numbers */
enum RtT_Types {
	R_FUSION_NONE		= 0,
	R_FUSION_32			= 1,
	R_FUSION_LI			= 2,
	R_FUSION_LUI		= 3,
	/* Not used yet
	 * R_FUSION_LI_PCREL = 4,
	 * R_FUSION_LUI_PCREL = 5,
	 */
	R_FUSION_SYS		= 6,
	R_FUSION_I			= 7,
	R_FUSION_RELATIVE	= 8,
	R_FUSION_LOAD		= 9,
	R_FUSION_STORE		= 10,
	R_FUSION_BRANCH		= 11,
	R_FUSION_JUMP		= 12,
	R_FUSION_JUMP_O		= 13
};

/* ELF Program Header */
typedef struct {
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
} Elf32_Phdr;

/* Text segment info Struct */
typedef struct{
	Elf32_Addr		entry;	/* Where entry point is */
	uint32_t		start;	/* Address where text segment starts */
	uint32_t		size;	/* Size of segment */
	uint32_t		end;	/* Address where text segment ends */
	uint32_t		align;	/* Alignment type for addresses */
} textseg_info;

/* Data segment info Struct */
typedef struct{
	uint32_t		start;	/* Address where data segment starts */
	uint32_t		end;	/* Address where data segment ends */
	uint32_t		size;	/* Size of segment */
	uint32_t		align;	/* Alignment type for addresses */
} dataseg_info;


/* Prototypes */
int elf_check_magnum(Elf32_Ehdr *hdr);
int elf_check_supported_arch(Elf32_Ehdr *hdr);
static inline Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr);
static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int i);
static inline char *elf_str_table(Elf32_Ehdr *hdr);
static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset);
static int elf_get_symval(Elf32_Ehdr *hdr, int table, unsigned int index);
static int elf_load_stage1 (Elf32_Ehdr *hdr);
static int elf_perform_reloc(Elf32_Ehdr *hdr, Elf32_Rela *rela, Elf32_Shdr *reltab);
static int elf_load_stage2(Elf32_Ehdr* hdr);
static inline void *elf_load_rel(Elf32_Ehdr *hdr);
void *elf_load_file(void *file);

#endif /* PARSEELF_H */
