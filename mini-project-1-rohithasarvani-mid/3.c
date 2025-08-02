#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "1.h"
#include "2.h"
#include "3.h"

#define MAX_PATH_LENGTH 4096
#define RED "\033[0;31m"
#define RESET "\033[0m"

char previous_dir[MAX_PATH_LENGTH] = ""; 
void hop_to_directory(const char *dir, char *home_dir) {
    char new_dir[MAX_PATH_LENGTH];
    char current_dir[MAX_PATH_LENGTH];

    
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        fprintf(stderr, RED "Error: getcwd failed\n" RESET);
        return;
    }

    if (dir[0] == '~') {
        if (strlen(dir) == 1) {
            
            if (chdir(home_dir) != 0) {
                fprintf(stderr, RED "Error: chdir to home directory failed\n" RESET);
                return;
            }
        } else {
           
            if (snprintf(new_dir, sizeof(new_dir), "%s/%s", home_dir, dir + 2) >= (int)sizeof(new_dir)) {
                fprintf(stderr, RED "Error: Path too long\n" RESET);
                return;
            }
            
            if (chdir(new_dir) != 0) {
                fprintf(stderr, RED "Error: chdir to expanded path failed\n" RESET);
                return;
            }
        }
    } else if (strcmp(dir, "-") == 0) {
        
        if (strlen(previous_dir) == 0) {
            fprintf(stderr, RED "Error: No previous directory stored\n" RESET);
            return;
        }
        if (chdir(previous_dir) != 0) {
            fprintf(stderr, RED "Error: chdir to previous directory failed\n" RESET);
            return;
        }
    } else if (strcmp(dir, ".") == 0) {
        
        return;
    } else if (strcmp(dir, "..") == 0) {
        
        if (chdir("..") != 0) {
            fprintf(stderr, RED "Error: chdir to parent directory failed\n" RESET);
            return;
        }
    } else {
        
        if (chdir(dir) != 0) {
            fprintf(stderr, RED "Error: chdir to specified path failed\n" RESET);
            return;
        }
    }

   
    strncpy(previous_dir, current_dir, sizeof(previous_dir) - 1);
    previous_dir[sizeof(previous_dir) - 1] = '\0';

    
    if (getcwd(new_dir, sizeof(new_dir)) != NULL) {
        printf("%s\n", new_dir);
    } else {
        fprintf(stderr, RED "Error: getcwd after chdir failed\n" RESET);
    }
}


void handle_hop_command(char *input, char *home_dir) {
    char *arg;
    char *saveptr;

    
    arg = strtok_r(input, " ", &saveptr);
    while (arg != NULL) {
        trim_whitespace(arg);
        if (strlen(arg) > 0) {
            hop_to_directory(arg, home_dir);
        }
        arg = strtok_r(NULL, " ", &saveptr);
    }
}
