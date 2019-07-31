#include "setup.h"
#include "memory.h"
#include "../fusion-elf/fusion-elf.h"

//int is_diff_endian = tgt_endian != host_endian;

#if 0
/* Creates memory map of ELF file
 * Takes filename as parameter
 * Saves filesize into respective global variable
 * Returns pointer to memory map
 * if error occurs, return NULL */
addr_8_t* open_elf_map( const char* filename ){
	/* Open ELF file */
	elf_file = open( filename, O_RDONLY, (mode_t)0600);	
	
	/* Check if file could be opened */
	if( elf_file == -1 ){
		elf_error = ELF_OPEN;	/* Set error, couldn't open file */
		return NULL;	
	}
	
	/* Get file size */
	struct stat file_info = {0};
	
	/* check if file could be read */
	if( fstat( elf_file, &file_info) == -1 ){
		elf_error = ELF_SIZE;	
		return NULL;
	} 

	/* Check if file is empty */
	if( file_info.st_size == 0){
		elf_error = ELF_EMPTY;	
		return NULL;
	}
	/* Save filesize */
	filesize = file_info.st_size;

	/* perform map and return pointer */
	return mmap(0, file_info.st_size, PROT_READ, MAP_SHARED, elf_file, 0);
}

/* Frees memory map */
void close_elf_map( addr_8_t* map, intmax_t filesize){
	/* unmap and check if unmapped */
	if( munmap(map, filesize) == -1){
		elf_error = ELF_FREE;
	}
	close(elf_file);
}
int setup_insns( uint32_t *elf_insn, uintmax_t insn_cnt ){
	//if( is_diff_endian == 0 ) {	
		for( int i = 0; i < insn_cnt; i++){
			insn_mem[i] = elf_insn[i];
		}
	} else { /* Only matters if there is a difference */
		/* Fix instructions for execution to be opposite endian */		
		int tmp_insn = 0;	/* Temporary value for instruction */
		for( int i = 0; i < insn_cnt; i++){
			insn[i] = ENDSWP_32( elf_insn[i] ); 	
		}

	}
	return 0;
}
/* Fix the data segment to the correct endian
 * Data needs to be aligned at 4 or 16 bytes, 
 * to ensure that this change will work. Otherwise,
 * errors will occur in the change */
int fix_data_align16( uint16_t *elf_data, uint32_t data_size ){
	for(uint32_t i; i < data_size; i++){
		data_mem[i] = ENDSWP_16(elf_data[i]);
	}
	return 0;
}

int fix_data_align32( uint32_t *elf_data, uint32_t data_size ){
	for(uint32_t i; i < data_size; i++){
		data_mem[i] = ENDSWP_32(elf_data[i]);
	}
	return 0;
}
#endif
/* Allocates and initializes memory pointer variables
 * returns error values for memory space creation
 * creates memory allocation for the following variables:
 * 	-memory_space
 * 	-dmem
 * 	-imem
 * 	-cprmem
 * 	-rdomem
 *
 * Only memory space is allocated, the others are pointers to within the
 * memory space. This is to save memory, and more accurately simulate
 * a real memory space.
 * 	*/
int create_memspace( const char* filename ){
	
	/* Setup memoy blocks */
	init_memory(memblocks, 2);
	
	/* creates map of file to read out to the allocated memory */
	addr_8_t* elf_tmp = open_elf_map( filename );
	
	/** setup ELF structs **/
	/* Gets ELF Header */
	Elf32_Ehdr* elf_hdr = (Elf32_Ehdr *)elf_tmp; 

	/* Check ELF magic number*/
	if( elf_check_magnum( elf_hdr ) ){
		printf("Not ELF File\n");
		close_elf_map( elf_tmp, filesize);
		return -1;
	}
	/* Check proper machine */
	if( elf_check_supported_arch( elf_hdr) ){
		printf("Incorrect architecture\n");		
		close_elf_map( elf_tmp, filesize);
		return -2;
	}

	/* Get number of sections */
	int elf_nsec = elf_hdr->e_shnum;

	/* Make array of ELF Section Headers */
	Elf32_Shdr* elf_shdr[elf_nsec];
	for( int i = 0; i < elf_nsec; i++){
		elf_shdr[i] = elf_section( elf_hdr, i );
	}

	/* Get number of program headers */
	int elf_nprg = elf_hdr->e_phnum;

	/* Variable for size of program */
	size_t prgmem_size = 0; 

	/* Make array of program headers, copy memory at the same time */
	Elf32_Phdr* elf_phdr[elf_nprg];	
	for( int i = 0; i < elf_nprg; i++){
		elf_phdr[i] = (Elf32_Phdr *)( (uint8_t*)elf_hdr + elf_hdr->e_phoff + (i * (int)sizeof(Elf32_Phdr) ));

		/* Check if segment should be loaded */
		if( elf_phdr[i]->p_type == 1){//PT_LOAD ){
			printf("Found loadable segment %d\nAddr: %08x\tSize: %08x\n", i, (unsigned int)elf_phdr[i]->p_vaddr, (unsigned int)elf_phdr[i]->p_filesz );

			/* Need to remove ELF header section if it is the segment to load */
			if( elf_phdr[i]->p_vaddr < ROM_START ){
				write_mem( ROM_START, (uint8_t*)elf_hdr + (elf_phdr[i]->p_offset) + ( ROM_START - elf_phdr[i]->p_vaddr + 8 ), elf_phdr[i]->p_filesz -  + ( ROM_START - elf_phdr[i]->p_vaddr + 8 ), 1 );
			}
			else {

				write_mem( elf_phdr[i]->p_vaddr, (uint8_t*)elf_hdr + (elf_phdr[i]->p_offset), elf_phdr[i]->p_filesz, 1 );
			}
		}
	}
	/* save entry point, no need for pointer */
	entry = (fusion_addr_t)( ( elf_hdr->e_entry ) );


#if 0
	/* Variables for easy accessing program header values */
	uint32_t vaddr;
	uint32_t paddr; 
	uint32_t filesz;
	uint32_t memsz; 
	uint32_t align; 
	uint32_t offset;

	char *tmp_secname; /* Temporary pointer for getting string name from table */
	
	union ptr2uint ptr_tmp; /* temporary conversion between uint and pointer*/

	/* Load segments */
	for( int i = 0; i < elf_nprg; i++){
		/* Check if loadable segment */
		if( elf_phdr[i]->p_type == 1){
			/* Get useful variables */
			 vaddr 	= elf_phdr[i]->p_vaddr; 	/* virtual address start */
			 paddr 	= elf_phdr[i]->p_paddr; 	/* physical address start */
			 filesz = elf_phdr[i]->p_filesz; 	/* segment file size */
			 memsz 	= elf_phdr[i]->p_memsz;		/* segment virtual size */
			 align 	= elf_phdr[i]->p_align;		/* Boundary to align by */
			 offset = elf_phdr[i]->p_offset;	/* Segment offset in file */

			/** Define pointers **/
			for(int j = 0; j < elf_nsec; j++){
				/* If offsets are equal, can use section name to figure out
				 * what it is */
				if( elf_shdr[j]->sh_offset == offset ){
					/* get pointer to name of section */
					tmp_secname = elf_lookup_string( elf_hdr, elf_shdr[j]->sh_name );
					/* if match, then mark the text segment */
					if( strncmp( tmp_secname, ".text", 5) == 0 ){
						ptr_tmp.uint = offset;
						imem = (fusion_addr_t *)ptr_tmp.ptr; /* setting beginning of instruction memory */	

						ptr_tmp.uint = offset + memsz;
						imem_end = (fusion_addr_t *)(ptr_tmp.ptr);

					/* do the same for the data segment */
					} else if( strncmp( tmp_secname, ".data", 5) == 0 ){
						ptr_tmp.uint = offset;
						dmem = (uint8_t *)(ptr_tmp.ptr); /* setting beginning of data memory */	

						ptr_tmp.uint = offset + memsz;
						dmem_end = (uint8_t *)(ptr_tmp.ptr);
					}
						}	
			}


			/* If loading, zero the memory and load data */
			for(int j = vaddr; j < memsz; j++){ /* NOTE: need to change for alignment */
				/* Zero memory */
				*(memory_space + j ) = 0x00;
			}	
			for(int j = 0; j < filesz; j++){
				/* Copy new data over */
				*(memory_space + j + vaddr) = ( *(elf_tmp + j + offset) );
			}
		}

	} 	
			
#endif
	/* Clean up memory map, don't need it after this point */
	close_elf_map(elf_tmp, filesize);
	return 0;
}

/* Frees the memory allocated for the memory space created by
 * create_memspace.
 * The same variables are affected here
 * */
int free_memspace (void){
	//free(memory_space);
	free_memory( memblocks, 2);
}


