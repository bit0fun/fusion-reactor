
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "elf_read.h"
#include "opc.h"  
#include "cpu_sim.h"

/* Simulation Parameters */
#define DMEM_SIZE	2^(14)	/* Data memory size (kb) for simulation */

  const char* insn_type;
  const char* insn_name;
  uint8_t rd;
  uint8_t rsa;
  uint8_t rsb;
  uint32_t pc;   /* Actual PC value */
  uint32_t entry_point;
  uint32_t imm;
  uint32_t* data_mem;
  uint32_t registers[32];
  uint32_t* insn_mem;
  uint32_t code_line_num;



/* Max window dimensions */
int parent_y, parent_x;

/* Making the windows Global */
WINDOW *menu_bar;
WINDOW *asm_view;
WINDOW *reg_view;
WINDOW *mem_view;


/* Sizes and placement for windows
*  The widths are percentages
*  Positions and heights are mostly absolute
*  It should be noted that the values accomodate borders*/

#define MENU_BAR_X	0
#define MENU_BAR_Y	0
#define MENU_BAR_W	100	/* Full width of window*/
#define MENU_BAR_H	2

#define ASM_VIEW_X	1
#define ASM_VIEW_Y	2
#define ASM_VIEW_W	50	/* Half width of window */
#define ASM_VIEW_H	100	/* Full height of window */

#define REG_VIEW_X	
#define REG_VIEW_Y
#define REG_VIEW_W
#define REG_VIEW_H

#define MEM_VIEW_X
#define MEM_VIEW_Y
#define MEM_VIEW_W
#define MEM_VIEW_H


/* Prototypes */
int check_run_key( char c );
int check_exit_key( char c );
int refresh_all( void );
int draw_borders( void );
int count_line_number( const char* filename );
int read_asm_file( const char* filename, char*** asm_text );
const char* get_line_asm_file( char*** asm_text, int linenum );
int display_asm( char*** asm_text, int line_start, int line_end, int line_max);
int print_reg(uint32_t* reg,  uint32_t pc);
int print_mem(uint32_t** mem, int xpos, int ypos, uint32_t addr_start, int naddr, uint32_t addr_seg_start);


int main(int argc, char** argv){

	/* Assembly File Text */
	char**	asm_text;
	int 	asm_line_count = 0;

	/* ELF File data */
	textseg_info im_info;
	dataseg_info dmem_info;

	/* Memory Data */
	data_mem = calloc( DMEM_SIZE, sizeof( uint32_t ) );	
	insn_mem = NULL; /* needs to be allocated after getting ELF file */


	
	/* Get assembly file to display */
	if(argc != 2) {
		endwin();
		printf("Usage: fusion-reactor <file.elf>\n ");
		return -1;
	}

	/* reduce size, since it isn't necessary for such a large buffer */
	//int asm_file_len = strlen( argv[1] );
	//char* asm_filename = (char*) calloc(asm_file_len, sizeof(char));
	//strncpy(asm_filename, (const char*) argv[1] , (size_t) asm_file_len);
	
	//asm_line_count = count_line_number( asm_filename );
	//read_asm_file(asm_filename, &asm_text);
	
	/*Create objdump file to be printed in code area */
	char create_objdump_file_command[512] = "fusion-elf-objdump -d \0";
	//if( sizeof( argv[2] ) > 478 ){
	//	return -1;
	//}
	strncat(create_objdump_file_command, argv[1], 478);
	strncat(create_objdump_file_command, " >> code.txt\0", 13);
	if(system(create_objdump_file_command))
		printf("Could not create objdump file");	  
	
    asm_line_count = count_line_number( "code.txt" );
	read_asm_file("code.txt", &asm_text);

//	int elf_file_len = strlen( argv[2] );
//	char* elf_filename = (char*) calloc(asm_file_len, sizeof(char));
//	strncpy(elf_filename, (const char*) argv[1] , (size_t) asm_file_len);


	/* Initializing ELF File */
	int result = read_elf(argv[1], &insn_mem, data_mem, &im_info, &dmem_info);
	if( result ){
		printf("Cannot parse elf\n");
	}


	entry_point = im_info.entry;
	int cycleno = 0;
	pc = im_info.entry;
    code_line_num = 0;
	char* code_win_title = "Assembly Code";
	char input;
	insn_info insn_i;
/* Debugging memory passed */
/*	printf("\nPassed memory\n");
	for(uint32_t i = ((pc - entry_point)/4); i < (im_info.end - im_info.entry)/4 ; i++){
		printf("@:%08x |\t %08x\n", ((i*4)+entry_point), (insn_mem)[i]);
	}
*/

	uint32_t imem_offset = entry_point - im_info.start; /* needed to find proper starting position */
	code_line_num += (pc - entry_point + imem_offset) /4;
	insn_i.word = insn_mem[ code_line_num ]; /* Saving instruction word */
	uint32_t old_pc = 0;
	while(1){
		/* Stepping through program */
		old_pc = pc; /* save old PC */
		printf("\nInsn:\t%08x\n",insn_i.word );
//		printf("PC:\t%08x\n",pc);
		result = execute( &insn_i, registers, &pc, &cycleno, &data_mem, dmem_info );
		printf("Registers:\n");
		print_reg( registers, pc);
		/* Used until ABI is created for syscall exiting program */
		if( (pc - entry_point)/4 >= (im_info.end - im_info.entry)/4 ) {
			printf("Program exit. Number of cycles: %d", cycleno);
			return 0;
		}
		if( result == -1 ){
			printf("Internal Error. Halting execution\n");
			break;
		} else if( result == EM_INT ){
			printf("Invalid Arithmetic Instruction.\n");
		} else if( result == EM_IMM ){
			printf("Invalid Immediate Instruction.\n");
		} else if( result == EM_LD ){
			printf("Invalid Load Instruction.\n");
		} else if( result == EM_ST ){
			printf("Invalid Store Instruction.\n");
		} else if( result == EM_LI ){
			printf("Invalid Load Immediate Instruction.\n");
		} else if( result == EM_B ){
			printf("Invalid Branch Instruction.\n");
		} else if( result == EM_SYS ){
			printf("Invalid System Instruction.\n");
		} else if( result == EM_NCPID ){
			printf("Invalid Co-Processor ID.\n");
		} else if( result == EM_CP ){
			printf("Co-Processor Instructions are not available.\n");
		} else {
			printf("Proper Instruction\n");
		}
		code_line_num = (pc - entry_point + imem_offset )/4;
		insn_i.word = insn_mem[ code_line_num ]; /* Saving instruction word */
		printf("updated pc\n");
	}
	return 0;

}
/* Returns total number of lines in a file */
int count_line_number( const char* filename ){
	FILE* fp = fopen(filename, "r");
	if(fp == NULL){
		//mvwprintw(asm_view, 1, 3, "Can't find number of lines in file");	
		fclose(fp);
		return -1;
	}
	char c; /* current character reading from file */
	int numline = 0; /* number of lines, counter */
	do{
		/* read character */
		c = getc(fp);	
		if( c == '\n')
			numline++; /* end of line, so increment */
	} while(c != EOF); /* Loop until EOF*/
	fclose(fp);
	//mvwprintw(asm_view, 1, 3, "Number of lines in file: %d", numline);
	return numline;
}



/* Read entirety of assembly file
 * Function will only read 80 characters of each line, so write
 * your damn code well
 * */
int read_asm_file( const char* filename, char*** asm_text ){
	int numlines = count_line_number(filename);
	int i;

	/* allocating the string array */
//	asm_text = malloc( sizeof(char**) );
//	char** text = malloc( sizeof(char*) * numlines * 80);
	*(asm_text) = (char**) malloc( sizeof(char*) * numlines);

	//if( text == NULL ){
	if( (*asm_text == NULL) ) {
	//	mvwprintw(asm_view, 1, 3, "Memory allocation issue for assembly file array");
		//free(text);
		free(*asm_text);
		free(asm_text);
		return -1;
	}

	for( i = 0; i < numlines; i++){
//		*(text + i) = (char*) calloc(80, sizeof(char)); /* 80 characters */
		(*asm_text)[i] = (char* )calloc(80, sizeof(char)); /* 80 characters */
		//if( *(text + i) == NULL){
		if( (*asm_text)[i] == NULL){
		//	mvwprintw(asm_view, 1, 3, "Memory allocation issue on line: %d", i);
			return -1;
		}
	}

	/* opening file to read from */
	FILE *asm_file = fopen(filename, "r");
	if(asm_file == NULL){
	//	mvwprintw(asm_view, 1, 3, "Can't read assembly file");
		fclose(asm_file);
		return -1;
	}

	/* actually reading file */
	char c; /* current character reading from file */
	int cline = 0; /* current line */
	int chrnum = 0; /* character count per line */
	do {
		/* read character */
		c = getc(asm_file);	
		if( c == '\n') {
			cline++; /* end of line, so increment */
			chrnum = 0; /* reset character count */
		} else if( c == EOF ){
			break;
		}
		else{
//			text[cline][chrnum] = c;
			(*asm_text)[cline][chrnum] = c;

			chrnum++;
		}
	} while(c != EOF); /* Loop until EOF*/

	//mvwprintw(asm_view, 1, 3, "Completed reading file:\n%s", filename);
	
	/* Sending temp to output */
//	asm_text = &text;

	fclose(asm_file);
	return 0;
}

/* Reads one line from assembly file */
const char* get_line_asm_file( char*** asm_text, int linenum ){
	return (const char* ) **(asm_text + linenum);
}

/* Reads assembly text and displays it in the asm_view window */
int display_asm( char*** asm_text, int line_start, int line_end, int line_max){

	if(line_end > line_max){
		line_end = line_max;	
	}
	if(line_start < 0){
	//	mvwprintw(asm_view, 1, 3, "Invalid line start number");
		return -1;
	}
	int wlinenum = (line_end - line_start);
	/* Figure out if there is enough space to write in window */
	if( wlinenum > (parent_y - MENU_BAR_H) ){
		/* Fix to be maximum amount of lines */
		wlinenum = (parent_y - MENU_BAR_H);	
	}

	int i;
	/* Write out lines */
	for(i = line_start; i < wlinenum; i++){
		if( i == code_line_num - line_start)
		 // mvwprintw(asm_view, (3+ i), 2, "*%s\n", (*asm_text)[i+line_start]);
		  mvwprintw(asm_view, (3+(i - line_start)), 2, "*%s\n", (*asm_text)[i]);
		else
		  //mvwprintw(asm_view, (3+ i), 3, "%s\n", (*asm_text)[i+line_start]);			
		  mvwprintw(asm_view, (3+(i - line_start)), 3, "%s\n", (*asm_text)[i]);			
	}

	return 0;
}

/* Prints out register values to proper window
I * any more register values can be added if needed, though unsure of what to
 * add
 * */
int print_reg(uint32_t* reg,  uint32_t pc){
	int i;
	
	for(i = 0; i < 16; i++){
	//	mvwprintw(reg_view, i, 1, "r[%d]:\t%08x", i, reg[i]);			
	//	mvwprintw(reg_view, i, 18, "r[%d]:\t%08x", i+16, reg[i+16]);			
		printf("r[%d]:\t%08x\tr[%d]:\t%08x\n", i, reg[i], i+1, reg[i+16]);
	}
	printf("Special Registers\n");
	printf("PC:\t%08x\n\n", pc);
	//mvwprintw(reg_view, 1, 41, "pc:\t%08x", pc);			



}

/* Prints out memory to memory view
 * mem is the actual array for the memory
 * xpos and ypos refer to the position within the window to print out
 * addr_start is the address to start printing from
 * naddr is the number of addresses to print out
 * addr_seg_start is the address start of the segment 
 * */
int print_mem(uint32_t** mem, int xpos, int ypos, uint32_t addr_start, int naddr, uint32_t addr_seg_start){
	uint32_t i;
	if(addr_start <= addr_seg_start){
		i = 0;
	} else{
		i = (addr_start - addr_seg_start) / 4;
	}
	
	for(i; i < naddr; i++){	
		mvwprintw(mem_view, i+ypos, 1+xpos, "(%08x) %08x", (addr_start + (i*4)), (*mem)[i]);
		mvwprintw(mem_view, i+ypos, 21+xpos, "|");
		mvwprintw(mem_view, i+ypos, 22+xpos, "%c%c%c%c", ( (char)(*mem)[i] & 0x000000ff ), (char)( (*mem)[i] & 0x0000ff00 ),
													     ( (char)(*mem)[i] & 0x00ff0000 ), (char)( (*mem)[i] & 0xff000000 ));
		mvwprintw(mem_view, i+ypos, 26+xpos, "|");
	} 

}
