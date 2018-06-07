#include "parseelf.h"
#define HOST_LITTLE_ENDIAN
int get_ehdr(const char* filename, Elf32_Ehdr* elf_hdr);
int get_shdr(const char* filename, Elf32_Shdr* sec_hdr, Elf32_Ehdr* elf_hdr, int index);
int get_phdr(const char* filename, Elf32_Phdr* prg_hdr, Elf32_Ehdr* elf_hdr, int index);

int get_section_data(const char* filename, Elf32_Shdr* sec_hdr, uint32_t** section_data );
int get_segment_data(const char* filename, Elf32_Phdr* prg_hdr, uint32_t** segment_data, int iftext );
uint32_t change_endian( uint32_t src );

int free_elf_data(uint32_t** text_seg, uint32_t** data_seg, Elf32_Ehdr* elf_hdr);
int	read_elf( const char* filename, uint32_t** text_segp, uint32_t** data_segp, textseg_info* tseg_i, dataseg_info* dseg_i );

