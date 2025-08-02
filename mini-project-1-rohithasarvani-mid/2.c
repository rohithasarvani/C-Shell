#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include "7.h" 
#include "3.h" 
#include "4.h" 
#include "5.h" 
#include "8.h" 
#define PURPLE "\033[0;35m"
#define RED "\033[0;31m"
#define RESET "\033[0m"


void trim_whitespace(char *str) {
    char *end;

    
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return; 

   
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    
    *(end + 1) = 0;
}


void execute_command(char *command, char *home_dir) {
    pid_t pid;
    int status;
    clock_t start;
    clock_t end;
    double elapsed_time;

    start = clock();

    
    add_to_log(command, home_dir);

    
    char *argv[64]; 
    int i = 0;

    
    char *token = strtok(command, " ");
    while (token != NULL && i < (int)(sizeof(argv) / sizeof(argv[0])) - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    if ((pid = fork()) == -1) {
        fprintf(stderr, RED "Error: fork failed\n" RESET);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        start = clock();
        execvp(argv[0], argv); 
        fprintf(stderr, RED "Error: execvp failed for command '%s'\n" RESET, argv[0]);
        exit(EXIT_FAILURE);
    } else { 
        waitpid(pid, &status, 0);
        end = clock();
        elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
        if (elapsed_time > 2.0) {
            printf("\nProcess %s took %.2f seconds to complete.\n", argv[0], elapsed_time);
        }
    }
}


void execute_command_in_background(char *command, char *home_dir) {
    pid_t pid;
    int status;

    
    add_to_log(command, home_dir);

    
    char *argv[64]; 
    int i = 0;

    
    char *token = strtok(command, " ");
    while (token != NULL && i < (int)(sizeof(argv) / sizeof(argv[0])) - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL; 

    if ((pid = fork()) == -1) {
        fprintf(stderr, RED "Error: fork failed\n" RESET);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        execvp(argv[0], argv); 
        fprintf(stderr, RED "Error: execvp failed for command '%s'\n" RESET, argv[0]);
        exit(EXIT_FAILURE);
    } else { 
        printf("Background PID: %d\n", pid);

        
        int child_pid;
        while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            if (WIFEXITED(status)) {
                printf("Process %d exited normally\n", child_pid);
            } else if (WIFSIGNALED(status)) {
                printf("Process %d exited with signal %d\n", child_pid, WTERMSIG(status));
            }
        }
    }
}


void process_commands(char *input, char *home_dir) {
    char *command;
    char *saveptr;

    command = strtok_r(input, ";", &saveptr);
    while (command != NULL) {
        trim_whitespace(command); 
        if (strlen(command) > 0) {
            
            if (strncmp(command, "hop", 3) == 0) {
                add_to_log(command, home_dir);
                handle_hop_command(command + 4, home_dir); 
            } else if (strncmp(command, "reveal", 6) == 0) {
                add_to_log(command, home_dir);
                handle_reveal_command(command + 7); 
            } else if (strncmp(command, "log", 3) == 0) {
                trim_whitespace(command + 4); 
                handle_log_command(command + 4, home_dir); 
            } else if (strncmp(command, "seek", 4) == 0) {
                add_to_log(command, home_dir);
                handle_seek_command(command + 5,home_dir); 
            } else if (strncmp(command, "proclore", 8) == 0) {
              
                  
                char *arg = command + 9;  
                trim_whitespace(arg);
               
                if (*arg != '\0') {  
                    printf("Argument found: %s\n", arg);
                    proclore(2, &arg); 
                } else {
                    printf("No argument provided.\n");
                    proclore(1, NULL); 
                }
            } else if (strchr(command, '&') != NULL) {
                add_to_log(command, home_dir);
                char *bg_command = strtok(command, "&");
                while (bg_command != NULL) {
                    trim_whitespace(bg_command);
                    if (strlen(bg_command) > 0) {
                        execute_command_in_background(bg_command, home_dir);
                    }
                    bg_command = strtok(NULL, "&");
                }
            } else {
                add_to_log(command, home_dir);
                execute_command(command, home_dir);
            }
        }

        command = strtok_r(NULL, ";", &saveptr);
    }
}
