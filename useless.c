#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

void useless_usage(void);
int  plan_sort(const char *path);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        useless_usage();
        exit(EXIT_FAILURE);       
    }
	
	printf("%d\n",plan_sort(argv[1]));
	
	return 0;
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

int  plan_sort(const char *path) {
	
	pid_t pid = 0;
	int retStatus = 0 ,temp = 0, file = 0;
	
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	file = creat("/tmp/useless_plan[sorted].txt", mode);					
																		
	if (file == -1) {
		printf("Failed to create file \"/tmp/useless_plan[sorted].txt\".\nReason: %s.\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	switch(pid) {
		
			case -1:
				printf("Something wrong with fork()\nReason: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
				break;
				
			case 0:
				temp = execlp("sort", "sort", "-k1", path, "-o", "/tmp/useless_plan[sorted].txt", (char *)0);
				if (temp == -1) {
					printf("Something wrong with exelp()\nReason: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				break;
				
			default:
				temp = waitpid(pid, &retStatus, WNOHANG);
				if (temp == -1) {
					printf("Something wrong with waitpid()\nReason: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				break;
	}
	
	close(file);
	
	if (WIFEXITED(retStatus)) 	return  0;								// If WIFEXITED returns  TRUE it means that child terminated successfully
	else 						return -1;
}

