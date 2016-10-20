/************************************************************************
*     * File: useless.c                                                 *
*     * Description:UNIX System Extreamly Late Execution Software System*
*     * Created: 30.09.2014                                             *
*     * Copyright: (C) MIPT                                             *
*     * Author: Karim Iskakov                                           *
*     * Email: karfly@yandex.ru                                         *                                                                   *
*************************************************************************/

#include "useless.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[]) {

    int temp = 0, i = 0, exitFlag = EXIT_FAILURE;

    FILE* plan = NULL;

    char **args = NULL;

    double time_shift = 0;

    struct Program *progs_info = NULL;

    if (argc != 2) {
        useless_usage();
        exit(EXIT_FAILURE);
    }

    // +++++ OPENING FILES AND ALLOCATING MEMORY +++++ \\

    temp = plan_sort(argv[1]);
    if (temp == -1) {
        printf("Something wrong with sorting\n");
        goto exit_main;
    }

    plan = fopen("/tmp/useless_plan[sorted].txt", "r");
    if (plan == NULL) {
        printf("Failed to open file \"/tmp/useless_plan[sorted].txt\".\nReason: %s.\n", strerror(errno));
        goto exit_main;
    }

    int args_numb = 0;

    args = (char **)calloc(MAX_ARG_NUMB, sizeof(char *));
    if (args == NULL) {
        printf("Failed to allocate memory\nReason: %s.\n", strerror(errno));
        goto exit_main;
    }
    for(i = 0; i < MAX_ARG_NUMB; i++) {
        args[i] = (char *)calloc(MAX_ARG_SIZE, sizeof(char));
        if (args[i] == NULL) {
            printf("Failed to allocate memory\nReason: %s.\n", strerror(errno));
            goto exit_main;
        }
    }

    progs_info = (struct Program *)calloc(MAX_PROG_NUMB, sizeof(struct Program));
    for(i = 0; i < MAX_PROG_NUMB; i++) {
        progs_info[i].prog_name = (char *)calloc(MAX_PROG_NAME_SIZE, sizeof(char));
    }

    // ++++++ MAIN PART OF PROGRAM ++++++ \\

    pid_t pid = 0, ret_pid = 0;
    double prev_time = 0;
    int numb_of_proc = 0, numb_of_proc_run = 0;
    int max_numb_of_proc = MAX_PROG_NUMB;
    int retStatus = 0;

    while ((temp = read_string(plan, &time_shift, &args_numb, args)) != 0) {

        if (temp == -1) {
            goto exit_main;
        }

        usleep((int)((time_shift - prev_time) * 1000000));
        prev_time = time_shift;

        pid = fork();
        switch(pid) {

            case -1:
                printf("Something wrong with fork()\nReason: %s\n", strerror(errno));
                goto exit_main;
                break;

            case 0:
                args[args_numb] = NULL;
                temp = execvp(args[0], args);
                if (temp == -1) {
                    printf("Something wrong with execvp()\nReason: %s\n", strerror(errno));
                    goto exit_main;
                }
                break;

            default:
                numb_of_proc_run++;

                if (numb_of_proc >= max_numb_of_proc) {
                    progs_info = realloc(progs_info, max_numb_of_proc * 2 * sizeof(struct Program));
                    if (progs_info == NULL) {
                        printf("Failed to allocate memory\nReason: %s.\n", strerror(errno));
                        goto exit_main;
                    }
                    max_numb_of_proc = max_numb_of_proc * 2;
                }

                progs_info[numb_of_proc].pid = pid;
                strcpy(progs_info[numb_of_proc].prog_name, args[0]);
                progs_info[numb_of_proc].time_shift = time_shift;
                //printf("%s %lg %d\n%s %lg %d\n", args[0], time_shift, pid, progs_info[numb_of_proc].prog_name, progs_info[numb_of_proc].time_shift, progs_info[numb_of_proc].pid);
                numb_of_proc++;

                ret_pid = waitpid(-1, &retStatus, WNOHANG);
                if (ret_pid == -1) {
                    printf("Something wrong with waitpid()\nReason: %s\n", strerror(errno));
                    goto exit_main;
                }
                if (ret_pid > 0) {
                    numb_of_proc_run--;
                    for (i = 0; i < numb_of_proc; i++) {
                        if (ret_pid == progs_info[i].pid) {
                            printf("Program (\033[31m%s\033[0m) with time shift (\033[31m%lg\033[0m sec) and pid (\033[31m%d\033[0m) exited with status: \033[31m%d\033[0m\n", progs_info[i].prog_name, progs_info[i].time_shift, progs_info[i].pid, WEXITSTATUS(retStatus));
                        }
                    }
                }
                break;
        }

    }

    // +++++ KILLING ZOMBIES +++++ \\

    while (numb_of_proc_run != 0) {
        ret_pid = wait(&retStatus);
        if (ret_pid == -1) {
            printf("Something wrong with waitpid()\nReason: %s\n", strerror(errno));
            goto exit_main;
        }
        if (ret_pid > 0) {
                numb_of_proc_run--;
                for (i = 0; i < numb_of_proc; i++) {
                    if (ret_pid == progs_info[i].pid) {
                        printf("Program (\033[31m%s\033[0m) with time shift (\033[31m%lg\033[0m sec) and pid (\033[31m%d\033[0m) exited with status: \033[31m%d\033[0m\n", progs_info[i].prog_name, progs_info[i].time_shift, progs_info[i].pid, WEXITSTATUS(retStatus));
                    }
                }
        }
    }

    exitFlag = EXIT_SUCCESS;
    exit_main:

        for(i = 0; i < MAX_ARG_NUMB; i++) {
            if (args[i] != NULL) {
                free(args[i]);
            }
        }
        if (args != NULL) {
            free(args);
        }

        for(i = 0; i < MAX_PROG_NUMB; i++) {
            if (progs_info[i].prog_name != NULL) {
                free(progs_info[i].prog_name);
            }
        }
        if (progs_info != NULL) {
            free(progs_info);
        }

        if (plan != NULL){
            fclose(plan);
        }

        exit(exitFlag);
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
        return -1;
    }
    close(file);

    pid = fork();
    switch(pid) {

            case -1:
                printf("Something wrong with fork()\nReason: %s\n", strerror(errno));
                return -1;
                break;

            case 0:
                temp = execlp("sort", "sort", "-k1", "-n", path, "-o", "/tmp/useless_plan[sorted].txt", (char *)0);
                if (temp == -1) {
                    printf("Something wrong with exelp()\nReason: %s\n", strerror(errno));
                    return -1;
                }
                break;

            default:
                temp = waitpid(pid, &retStatus, 0);
                if (temp == -1) {
                    printf("Something wrong with waitpid()\nReason: %s\n", strerror(errno));
                    return -1;
                }
                break;
    }

    if (WIFEXITED(retStatus))   return  0;                              // If WIFEXITED returns TRUE it means that child terminated successfully
    else                        return -1;
}

int read_string(FILE *file, double *time_shift, int *argc, char **argv) {

    char c = 0;
    int i = 0, j = 0, k = 0 ,retFlag = -1;
    char *string_r = NULL;
    char *string = NULL;
    char *number = NULL;
    char *temp   = NULL;

    // Allocating memory
    string_r = (char *)calloc(MAX_STR_SIZE, sizeof(char));
    if (string_r == NULL) {
        printf("Failed to allocate memory for <string_r>\nReason: %s.\n", strerror(errno));
        goto exit_read_string;
    }

    string = (char *)calloc(MAX_STR_SIZE, sizeof(char));
    if (string == NULL) {
        printf("Failed to allocate memory for <string>\nReason: %s.\n", strerror(errno));
        goto exit_read_string;
    }
    strcpy(string_r, string);                                           // Copy to recover string then


    string = fgets(string, MAX_STR_SIZE - 1, file);
    if (string == NULL) {
        retFlag = 0;
        goto exit_read_string;
    }

    // Getting time shift
    number = strtok(string, " \t\n");
    if (number == NULL) {
        printf("Something wrong with plan file. Check it\n");
        goto exit_read_string;
    }
    *time_shift = atof(number);

    // Getting program name
    temp = strtok(NULL, " \t\n");
    if (number == NULL) {
        printf("Something wrong with plan file. Check it\n");
        goto exit_read_string;
    }
    strcpy(argv[0], temp);

    // Getting arguments
    i = 1;
    while ((temp = strtok(NULL, " \t\n")) != NULL) {
        strcpy(argv[i++], temp);
    }
    *argc = i;

    retFlag = 1;
    strcpy(string, string_r);                                           // Recovering string after strtok()
    exit_read_string:

        if (string != NULL) {
            free(string);
        }

        if (string_r != NULL) {
            free(string_r);
        }

        return retFlag;
}
