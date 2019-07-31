#ifndef SIM_SIG_H
#define SIM_SIG_H

/* Signals specific for operation of this simulator to provide helpful messages
 * to the user */
enum sim_err{
	SIM_NOERR,					/* No error */
	SIM_SEGFAULT,				/* Segmentation Fault */

};


/* Signals from *nix systems*/
enum sim_signals_nix{
	SIM_SIG_NONE	= 0,		/* No signal */
	SIM_SIG_HUP		= 1,		/* Hang-up, end of terminal or controlling process */
	SIM_SIG_INT		= 2,		/* Interrupt, from user */
	SIM_SIG_QUIT	= 3,		/* Quit, from user */
	SIM_SIG_FPE		= 8,		/* Floating point exception */
	SIM_SIG_KILL	= 9,		/* Force kill process */
	SIM_SIG_ALRM	= 14,		/* Alarm clock signal */
	SIM_SIG_TERM	= 15,		/* Software termination signal */

};

#endif
