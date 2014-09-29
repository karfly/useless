#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void useless_usage(void);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        useless_usage();
        exit(EXIT_FAILURE);       
    }


}

void useless_usage(void) {
    printf(
    "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
    "USELESS (UNIX System Extreamly Late Execution Software System).\n"
    "It allows you to start different programs in some delay, accorging\n"
    "to the plan of execution.\n\n"
    "SYNOPSIS\n"
    "\tuseless [FILE]\n\n"
    "DESCRIPTION\n"
    "\tFILE -is a text file, where the plan of starting programs is written\n\n"
    "FORMAT\n"
    "\t[FILE]:\n"
    "\t1.[SHIFT in secs] [PROG_NAME] [ARG(1)] ...[ARG(N)]\n"
    "\t2.[SHIFT in secs] [PROG_NAME] [ARG(1)] ...[ARG(N)]\n"
    "\t...\n"
    "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
    );
}

