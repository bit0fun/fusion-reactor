bin_PROGRAMS = fusion_reactor
fusion_reactor_SOURCES = 	main.c 		\
							memory.c	\
							memory.h	\
							memcfg.h	\
							simtypes.c	\
							simtypes.h	\
							setup.c		\
							setup.h		\
						 	opc.c 		\
							opc.h		\
							cpu_sim.c	\
							cpu_sim.h	\
							simsig.h	\
							fusion.h	\
							fusion-opc.h \
							emul_error.h
fusion_reactor_CFLAGS = -I $(top_srcdir)/fusion-elf
fusion_reactor_LOAD = -L $(top_srcdir)/fusion-elf/libfusionelf.a 
fusion_reactor_LDADD = ../fusion-elf/libfusionelf.a
