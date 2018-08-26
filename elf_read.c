#include "elf_read.h"
int	read_elf( const char* filename, uint32_t** text_segp, uint32_t** data_segp, textseg_info* tseg_i, dataseg_info* dseg_i ){

	Elf32_Ehdr* elf_hdr = malloc( sizeof(Elf32_Ehdr) ); 

	int result = get_ehdr(filename, elf_hdr);
	if (result <0){
		return -1;
		printf("Elf parsing failed\n");
	}
	uint32_t test = elf_hdr->e_machine;

	if(elf_check_supported_arch(elf_hdr)){	
		printf("Unsupported ELF file type");	
		return -1;
	}
	
	/* Create array of section headers to get from ELF file */
	Elf32_Shdr* section_headers[ elf_hdr->e_shnum ];

	for(int i = 0; i < elf_hdr->e_shnum; i++){
		section_headers[i] = malloc( sizeof(Elf32_Shdr) );
	 	get_shdr(filename, (section_headers[i]), elf_hdr, i);
	}
	 
	/* Print out section header types for debugging */
//	for(int i = 0; i < elf_hdr->e_shnum; i++){
		//printf("Section Header %d is of type: %d\n", i,(section_headers[i]->sh_type));
		//printf("Section is at offset: %08x\n\n", (section_headers[i]->sh_offset));
		//if( ((section_headers[i]->sh_type) == SHT_PROGBITS) && ((section_headers[i]->sh_flags) & SHF_EXECINSTR ))
//	} 

	/* Create Program Header Table, number of entries found in e_phnum */
	//printf("There are %d program headers in this file.\n", elf_hdr->e_phnum);
	Elf32_Phdr* program_headers[ (elf_hdr->e_phnum) ];
	for(int i = 0; i < (elf_hdr->e_phnum); i++){
		program_headers[i] = malloc( sizeof(Elf32_Phdr) );
		//printf("getting program header number %i\n", i);
		get_phdr(filename, (program_headers[i]), elf_hdr, i);
	}

	/* Allocating Text segment */
	//uint32_t* text_seg = malloc( (size_t) (program_headers[0]->p_filesz));
	*text_segp = malloc( (size_t) (program_headers[0]->p_filesz));
		

	//get_segment_data(filename, (program_headers[0]), &text_seg );
	get_segment_data(filename, (program_headers[0]), text_segp, 1 );
	//printf("Finished allocating text_segment\n");

	/* Getting total number of instructions*/
	int num_insn = ( (program_headers[0]->p_filesz) / (4));
	//printf("Numer of instructions: %d\n", num_insn);
	//printf("Text segment size: %08x\n", (unsigned long int) (program_headers[0]->p_filesz));


	/* Allocating Data segment */
	//uint32_t* data_seg = malloc( (size_t) (program_headers[1]->p_filesz));
	*data_segp = malloc( (size_t) (program_headers[1]->p_filesz));
	
//	get_segment_data(filename, (program_headers[1]), &data_seg );
	get_segment_data(filename, (program_headers[1]), data_segp, 0);
	//printf("Finished allocating data_segment\n");

	uint32_t pc = elf_hdr->e_entry; /* entry point of program */
	uint32_t text_start = program_headers[0]->p_paddr; /* virtual address start */
	uint32_t text_end = text_start + (uint32_t)(program_headers[0]->p_filesz); /* end of text segment */
//	uint32_t num_insn = (text_end - text_start)/4;

	uint32_t data_start = program_headers[1]->p_paddr;
	uint32_t data_end	= data_start + (uint32_t)(program_headers[1]->p_filesz);


	printf("Entry point:\t0x%08x\n",(unsigned long int) pc);
	printf("Text address start:\t0x%08x\n", (unsigned long int) text_start);
	printf("Text address end:\t0x%08x\n", (unsigned long int) text_end);
	printf("Data segment start:\t0x%08x\n", (unsigned long int) data_start);
	printf("Data segment end:\t0x%08x\n", (unsigned long int) data_end);

	/* Need to start at correct spot in array, so subtract the text segment
	 * start virtual address, from the program counter */
//	for(uint32_t i = (pc - text_start); i < num_insn; i++){
//	printf("Text segment\n");
//		printf("Virtual address: %08x\n", program_headers[0]->p_vaddr);
//		printf("Physical address: %08x\n", program_headers[0]->p_paddr);
//	for(uint32_t i = ((pc - text_start)/4); i < num_insn; i++){
//		printf("@:%08x |\t %08x\n", ((i*4)+text_start), (*text_segp)[i]);
//	}

//	printf("Data segment\n");
//	printf("Virtual address: %08x\n", program_headers[1]->p_vaddr);
//	printf("Physical address: %08x\n", program_headers[1]->p_paddr);

//	for(uint32_t i = 0; i < sizeof(data_seg); i++){
//			printf("@:%08x |\t %08x\n", ((i*4)+data_start), data_seg[i]);
//	}

	/* Final steps to pass data along */
//	*text_segp = text_seg;
//	*data_segp = data_seg;
	
	tseg_i->entry = elf_hdr->e_entry;
	tseg_i->start = text_start;
	tseg_i->end	  = text_end;
	tseg_i->size  = program_headers[0]->p_memsz;
	tseg_i->align  = program_headers[0]->p_align;

	dseg_i->start = data_start;
	dseg_i->end	  = data_end;
	dseg_i->size  = program_headers[1]->p_memsz;
	dseg_i->align  = program_headers[1]->p_align;

	for(int i = 0; i < elf_hdr->e_shnum; i++){
		free((section_headers)[i]);
	}
	for(int i = 0; i < elf_hdr->e_phnum; i++){
		free((program_headers)[i]);
	}
	free(elf_hdr);


	return 0;
}

int free_elf_data(uint32_t** text_seg, uint32_t** data_seg, Elf32_Ehdr* elf_hdr){
	free(*text_seg);

	free(*data_seg);


}

int get_ehdr(const char* filename, Elf32_Ehdr* elf_hdr){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open ELF file to parse Elf Header\n");
		return -1;
	}
	//void* tmp = (void *)(elf_hdr);	
	//void* tmp = (void *)malloc(sizeof(Elf32_Ehdr));
	//fread( tmp, sizeof(Elf32_Ehdr), 1, fp);
	fread( elf_hdr, sizeof(Elf32_Ehdr), 1, fp);

//	*elf_hdr = (Elf32_Ehdr *)(tmp);
#ifdef HOST_LITTLE_ENDIAN	
	/* Fixing 32 bit fields */
	(elf_hdr)->e_version	= change_endian((elf_hdr)->e_version);
	(elf_hdr)->e_entry 		= change_endian((elf_hdr)->e_entry);
	(elf_hdr)->e_phoff 		= change_endian((elf_hdr)->e_phoff);
	(elf_hdr)->e_shoff 		= change_endian((elf_hdr)->e_shoff);
	(elf_hdr)->e_flags 		= change_endian((elf_hdr)->e_flags);

	/* Fixing 16 bit fields */
	(elf_hdr)->e_type 		= (change_endian((elf_hdr)->e_type)) >> 16;
	(elf_hdr)->e_machine 	= (change_endian((elf_hdr)->e_machine)) >> 16;
	(elf_hdr)->e_ehsize 	= (change_endian((elf_hdr)->e_ehsize)) >> 16;
	(elf_hdr)->e_phentsize 	= (change_endian((elf_hdr)->e_phentsize)) >> 16;
	(elf_hdr)->e_phnum 		= (change_endian((elf_hdr)->e_phnum)) >> 16;
	(elf_hdr)->e_shentsize 	= (change_endian((elf_hdr)->e_shentsize)) >> 16;
	(elf_hdr)->e_shnum 		= (change_endian((elf_hdr)->e_shnum)) >> 16;
	(elf_hdr)->e_shstrnidx 	= (change_endian((elf_hdr)->e_shstrnidx)) >> 16;
#endif /* Host is Little Endian */
	return 0;
}

int get_shdr(const char* filename, Elf32_Shdr* sec_hdr, Elf32_Ehdr* elf_hdr, int index){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open ELF file to parse Section Header\n");
		return -1;
	}
	//void* tmp = (void *)malloc( sizeof(Elf32_Shdr));	
	/* Put file pointer to correct offset */
	//printf("(elf_hdr)->e_shoff = %08x \n", (elf_hdr)->e_shoff );
	fseek(fp, ( (elf_hdr)->e_shoff + (index * sizeof(Elf32_Shdr)) ), SEEK_SET);
	//fread( tmp, sizeof(Elf32_Shdr), 1, fp);
	fread( sec_hdr, sizeof(Elf32_Shdr), 1, fp);


	//*sec_hdr = (Elf32_Shdr *)(tmp);

#ifdef HOST_LITTLE_ENDIAN
	
	(sec_hdr)->sh_name 		= change_endian( (sec_hdr)->sh_name);
	(sec_hdr)->sh_type 		= change_endian( (sec_hdr)->sh_type );		
	(sec_hdr)->sh_flags 	= change_endian( (sec_hdr)->sh_flags );		
	(sec_hdr)->sh_addr	 	= change_endian( (sec_hdr)->sh_addr); 	
	(sec_hdr)->sh_offset	= change_endian( (sec_hdr)->sh_offset);	
	(sec_hdr)->sh_size 		= change_endian( (sec_hdr)->sh_size );	
	(sec_hdr)->sh_link 		= change_endian( (sec_hdr)->sh_link );
	(sec_hdr)->sh_info 		= change_endian( (sec_hdr)->sh_info );
	(sec_hdr)->sh_addralign	= change_endian( (sec_hdr)->sh_addralign	);
	(sec_hdr)->sh_entsize 	= change_endian( (sec_hdr)->sh_entsize);

#endif /* Host is Little Endian */
	//free(tmp);
	return 0;
}

int get_phdr(const char* filename, Elf32_Phdr* prg_hdr, Elf32_Ehdr* elf_hdr, int index){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open ELF file to parse Section Header\n");
		return -1;
	}
	//Elf32_Phdr* tmp = (void *)malloc( sizeof(Elf32_Phdr));	
	/* Put file pointer to correct offset */
	fseek(fp, ( (elf_hdr)->e_phoff + (index * sizeof(Elf32_Phdr)) ), SEEK_SET);
	//fread( tmp, sizeof(Elf32_Phdr), 1, fp);
	fread( prg_hdr, sizeof(Elf32_Phdr), 1, fp);

	//*prg_hdr = ((Elf32_Phdr *)(tmp));
	//**prg_hdr = tmp;
	/*
	(*prg_hdr)->p_type 		= tmp.p_type;
	(*prg_hdr)->p_offset	= tmp.p_offset;
	(*prg_hdr)->p_vaddr 	= tmp.p_vaddr;
	(*prg_hdr)->p_paddr 	= tmp.p_paddr;
	(*prg_hdr)->p_filesz	= tmp.p_filesz;
	(*prg_hdr)->p_memsz		= tmp.p_memsz; 
	(*prg_hdr)->p_flags		= tmp.p_flags; 
	(*prg_hdr)->p_align 	= tmp.p_align;
*/
//	printf("Before endian change, Segment offset: %08x\n", (prg_hdr)->p_offset);
#ifdef HOST_LITTLE_ENDIAN
	
	(prg_hdr)->p_type 	= change_endian( (prg_hdr)->p_type );
	(prg_hdr)->p_offset = change_endian( (prg_hdr)->p_offset );
	(prg_hdr)->p_vaddr  = change_endian( (prg_hdr)->p_vaddr );
	(prg_hdr)->p_paddr  = change_endian( (prg_hdr)->p_paddr );
	(prg_hdr)->p_filesz = change_endian( (prg_hdr)->p_filesz );
	(prg_hdr)->p_memsz  = change_endian( (prg_hdr)->p_memsz );
	(prg_hdr)->p_flags  = change_endian( (prg_hdr)->p_flags );
	(prg_hdr)->p_align 	= change_endian( (prg_hdr)->p_align );

#endif /* Host is Little Endian */
	//free(tmp);
	return 0;
}

/* Gets section data from file */
int get_section_data(const char* filename, Elf32_Shdr* sec_hdr, uint32_t** section_data ){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open ELF file to read section data.\n");
		return -1;
	}
	size_t sec_size = (size_t) (sec_hdr)->sh_size;

	if(sec_size <= 0){
		printf("No need to get section data with size of 0.\n");
		return -1;
	}

	//void* tmp = (void *)malloc( sec_size );	
	/* Put file pointer to correct offset */
	fseek(fp, ( (sec_hdr)->sh_offset), SEEK_SET);
	//fread( tmp, sec_size, 1, fp);
	fread( *section_data, sec_size, 1, fp);

#ifdef HOST_LITTLE_ENDIAN
	for(int i = 0; i < (int)(sec_size/4); i++){
		(*section_data)[i] = change_endian( (*section_data)[i] );
	}
#endif /* Host is Little Endian */
//	free(tmp);
	return 0;
}

/* Gets segment data from file */
int get_segment_data(const char* filename, Elf32_Phdr* prg_hdr, uint32_t** segment_data, int iftext ){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open ELF file to read section data.\n");
		return -1;
	}
	/* Size of segment in file */
	size_t seg_size = ((size_t) (prg_hdr)->p_filesz);

	if(seg_size <= 0){
		//printf("No need to get section data with size of 0.\n");
		return 1;
	}

//	void* tmp = (void *)malloc( seg_size );	
	/* Put file pointer to correct offset */
//	printf("Segment offset: %08x\n", (prg_hdr)->p_offset);
	fseek(fp, ( (prg_hdr)->p_offset), SEEK_SET);
//	fread( tmp, seg_size, 1, fp);
	fread( *segment_data, seg_size, 1, fp);


#ifdef HOST_LITTLE_ENDIAN
	for(uint32_t i = 0; i < (uint32_t)((seg_size)/4); i++){
		//(*segment_data)[i] = change_endian(* (((uint32_t *) tmp) + i));
		(*segment_data)[i] = change_endian( (*segment_data)[i] );
		//printf("Current segment data: %08x\n", (*segment_data)[i] );
	}
#endif /* Host is Little Endian */
//	free(tmp);
	return 0;
}

uint32_t change_endian( uint32_t src ){
	uint32_t b0, b1, b2, b3;
	b0 = (src & 0x000000ff) << 24u;
	b1 = (src & 0x0000ff00) << 8u;
	b2 = (src & 0x00ff0000) >> 8u;
	b3 = (src & 0xff000000) >> 24u;

	return (uint32_t)(b0 | b1 | b2 | b3);
}
