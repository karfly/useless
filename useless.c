#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define MAX_STR_SIZE 256

void useless_usage(void);
int  plan_sort(const char *path);
void read_string(FILE *file, double *time_shift, char *prog_name, char **argv);

int main(int argc, char *argv[]) {
    
    int temp = 0;
    int i = 0;
    char c = 0;
    
    if (argc != 2) {
        useless_usage();
        exit(EXIT_FAILURE);       
    }
    
	temp = plan_sort(argv[1]);
	if (temp == -1) {
		printf("Something wrong with sorting");
	}
	
	FILE* plan = fopen("/tmp/useless_plan[sorted].txt", "r");
	if (plan == NULL) {
		printf("Failed to open file \"/tmp/useless_plan[sorted].txt\".\nReason: %s.\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	char *prog_name = malloc(MAX_STR_SIZE);
	
	char **args = malloc(MAX_STR_SIZE);
	for(i = 0; i < MAX_STR_SIZE; i++) args[i] = malloc(MAX_STR_SIZE);
	
	double time_shift = 0;
	
	read_string(plan, &time_shift, prog_name, args);
	
	
	//printf("%lg %s %s\n", time_shift, prog_name, args[1]);
	
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
				temp = waitpid(pid, &retStatus, 0);
				if (temp == -1) {
					printf("Something wrong with waitpid()\nReason: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				break;
	}
	
	close(file);
	
	if (WIFEXITED(retStatus)) 	return  0;								// If WIFEXITED returns TRUE it means that child terminated successfully
	else 						return -1;
}

void read_string(FILE* file, double *time_shift, char *prog_name, char **argv) {
	
	int i = 0, j = 0, k = 0;
	
	char *number = NULL;
	number = malloc(MAX_STR_SIZE);											
	if (number == NULL) {
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	char *string = NULL;
	string = malloc(MAX_STR_SIZE);
	if (string == NULL) {
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	
	string = fgets(string, MAX_STR_SIZE - 2, file);
	
	if (string == NULL) {
		printf("Can't get another string from tmp/useless_plan[sorted].txt\n");
		free(number);
		free(string);
		exit(EXIT_FAILURE);
	}
	
	// Getting time shift 
	i = 0;
	k = 0;
	while (string[i] != ' ') {
		number[k++] = string[i++];
	}
	number[k] = '\0';
	i++;
	*time_shift = atof(number);
		
	// Getting prog name 
	k = 0;
	while (string[i] != ' ') {
		argv[0][k] = string[i];											// Because 1st arg is always the name of program
		prog_name[k++] = string[i++];
	}
	argv[0][k] = '\0';
	prog_name[k] = '\0';
	i++;
	
	
	// Getting args 
	j = 1;
	k = 0;
	while (string[i] != '\n'){
		while (string[i] != ' ' && string[i] != '\n') {
			argv[j][k++] = string[i++];
		}
		argv[j][k] = '\0';
		
		j++;
		if (string[i] == '\n') break;
		i++;
		k = 0;

    }
    argv[j] = NULL;
}
