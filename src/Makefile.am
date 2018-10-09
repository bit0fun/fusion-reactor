bin_PROGRAMS = fusion_reactor
fusion_reactor_SOURCES = 	main.c 		\
						 	opc.c 		\
							opc.h		\
							cpu_sim.c	\
							cpu_sim.h	\
							fusion.h	\
							fusion-opc.h \
							emul_error.h
fusion_reactor_CFLAGS = -I$(top_srcdir)/fusion-elf
fusion_reactor_LOAD = ../fusion-elf/libfusionelf.a
