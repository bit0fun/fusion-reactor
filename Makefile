CC=gcc
LDFLAGS= -l ncurses
CFLAGS= -g
PRGNAME=fusion-reactor
OBJ= main.o elf_read.o parseelf.o 
DEPS= elf_read.h parseelf.h fusion-opc.h fusion.h

all: $(PRGNAME)


$(PRGNAME): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
		
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -rf *.o *.gch

distclean:
	rm -rf *.o *.gch $(PRGNAME)
