CC=gcc
LDFLAGS= -lncurses 
CFLAGS= -g -std=c99
PRGNAME=fusion-reactor
OBJ= main.o elf_read.o parseelf.o opc.o cpu_sim.o
DEPS= elf_read.h parseelf.h fusion-opc.h fusion.h cpu.h opc.h cpu_sim.h

all: $(PRGNAME)

$(PRGNAME): $(OBJ)
	$(CC) $(CFLAGS)  -o $@ $^ $(LDFLAGS) 
		
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -rf *.o *.gch

distclean:
	rm -rf *.o *.gch $(PRGNAME)
