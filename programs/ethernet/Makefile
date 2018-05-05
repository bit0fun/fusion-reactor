CC=fusion-elf-as
AS=fusion-elf-as
LD=fusion-elf-ld
DEPS=
OBJ=hello.o

all: $(DEPS) link clean
	
link: $(OBJ)
	$(LD) $(OBJ) -o hello

%.S: 
	$(AS) $@.S -o $@.o
clean:
	rm -rf *.o
