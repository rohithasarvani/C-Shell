#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "2.h"
#include "7.h" 
#include "3.h" 
#include "4.h" 
#include "5.h" 
#include "8.h" 


#define MAX_LOG_SIZE 15
#define MAX_COMMAND_LENGTH 256
static char command_log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH];
static int log_count = 0;


void get_log_file_path(const char *home_dir, char *log_file_path) {
    snprintf(log_file_path, MAX_COMMAND_LENGTH, "%s/command_log.txt", home_dir);
}


void load_log(const char *home_dir) {
    char log_file_path[MAX_COMMAND_LENGTH];
    get_log_file_path(home_dir, log_file_path);
    
    FILE *file = fopen(log_file_path, "r");
    if (file == NULL) {
        return; 
    }

    log_count = 0;
    while (fgets(command_log[log_count], MAX_COMMAND_LENGTH, file) != NULL && log_count < MAX_LOG_SIZE) {
        size_t len = strlen(command_log[log_count]);
        if (len > 0 && command_log[log_count][len - 1] == '\n') {
            command_log[log_count][len - 1] = '\0';
        }
        log_count++;
    }
    fclose(file);
}


void save_log(const char *home_dir) {
    char log_file_path[MAX_COMMAND_LENGTH];
    get_log_file_path(home_dir, log_file_path);
    
    FILE *file = fopen(log_file_path, "w");
    if (file == NULL) {
        perror("Error opening log file for writing");
        return;
    }

    for (int i = 0; i < log_count; i++) {
        if (fprintf(file, "%s\n", command_log[i]) < 0) {
            perror("Error writing to log file");
            fclose(file);
            return;
        }
    }

    if (fflush(file) != 0) {
        perror("Error flushing log file");
    }

    if (fclose(file) != 0) {
        perror("Error closing log file");
    }
}


void add_to_log(const char *command, const char *home_dir) {
    if (strcmp(command, "log purge") == 0) {
        return; 
    }
    if (log_count > 0 && strcmp(command_log[log_count - 1], command) == 0) {
        return; 
    }

    
    if (log_count >= MAX_LOG_SIZE) {
        for (int i = 1; i < MAX_LOG_SIZE; i++) {
            strcpy(command_log[i - 1], command_log[i]);
        }
        log_count = MAX_LOG_SIZE - 1;
    }

    strncpy(command_log[log_count], command, MAX_COMMAND_LENGTH - 1);
    command_log[log_count][MAX_COMMAND_LENGTH - 1] = '\0'; 
    log_count++;

    save_log(home_dir);
}


void display_log(void) {
    for (int i = 0; i < log_count; i++) {
        printf("%d: %s\n", log_count - i, command_log[i]); 
    }
}


void clear_log(const char *home_dir) {
    log_count = 0; 
    save_log(home_dir); 
}


void execute_log_entry(int index, char *home_dir) {
    if (index < 1 || index > log_count) {
        fprintf(stderr, "Invalid log index\n");
        return;
    }

    int log_index = log_count - index;
    char *command = command_log[log_index];
    
    
    if (strncmp(command, "hop", 3) == 0) {
        handle_hop_command(command + 4, home_dir); 
    } else if (strncmp(command, "reveal", 6) == 0) {
        handle_reveal_command(command + 7); 
    } else if (strncmp(command, "seek", 4) == 0) {
        handle_seek_command(command + 5,home_dir); 
    } else if (strncmp(command, "proclore", 8) == 0) {
        
        char *args = command + 9; 
        char *argv[] = { "proclore", args }; 
        proclore(2, argv);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
    }
}



void handle_log_command(char *input, const char *home_dir) {
    
    trim_whitespace(input);

    if (strlen(input) == 0) {
        display_log();
        return;
    }

    
    char *command = strtok(input, " ");
    if (command == NULL) return;

    if (strcmp(command, "purge") == 0) {
        
        clear_log(home_dir);
    } else if (strcmp(command, "execute") == 0) {
        
        char *index_str = strtok(NULL, " ");
        if (index_str != NULL) {
            int index = atoi(index_str);
            execute_log_entry(index, (char *)home_dir);
        } else {
            fprintf(stderr, "No index provided for log execute\n");
        }
    } else {
        fprintf(stderr, "Unknown log sub-command: %s\n", command);
    }
}
