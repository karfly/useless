/************************************************************************
*     * File: useless.h						        *	
*     * Description:UNIX System Extreamly Late Execution Software System*			
*     * Created: 30.09.2014						*
*     * Copyright: (C) MIPT						*
*     * Author: Karim Iskakov						*
*     * Email: karfly@yandex.ru						*															*
*************************************************************************/


#ifndef USELESS_H
#define USELESS_H

#include <sys/types.h>
#include <stdio.h>

#define MAX_STR_SIZE 256
#define MAX_PROG_NAME_SIZE 64
#define MAX_ARG_SIZE 64
#define	MAX_ARG_NUMB 16
#define MAX_PROG_NUMB 128


struct Program {
	pid_t pid;
	char *prog_name;
	double time_shift;
};


	/**
     * Prints on display the manual
     */
void useless_usage(void);



	/**
     * Sort text file with plan using linux utilite SORT
     * 
     * @param	path	Absolute path to file, we want to sort
     * 
     * @note	Sorted file is situated here: "/tmp/useless_plan[sorted].txt\"
     * 
     * @return	0	Everything completed without errors
     * 			-1	If something went wrong. Prints the reason on screen
     */
int  plan_sort(const char *path);



	/**
     * Read another string from file and decodes it
     * 
     * @param	file		Text file with plan
     * @param	time_shift	Pointer to the variable, where time shift of program will be put
     * @param	argv		An array of strings, where 	
     * 
     * @note	Sorted file is situated here: "/tmp/useless_plan[sorted].txt\"
     * 
     * @return	-1	If an error occured
     * 			0	If end of file is reached
     * 			1	If string is read correctly
     */
int read_string(FILE *file, double *time_shift, int *argc, char **argv);

#endif
