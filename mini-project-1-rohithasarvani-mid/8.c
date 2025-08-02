#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "8.h"
#include "3.h"

#define MAX_PATH_LENGTH 4096
char cwd[MAX_PATH_LENGTH];

void print_colored(const char *path, int is_dir) {
    if (is_dir) {
        printf("\033[1;34m%s\033[0m\n", path); 
    } else {
        printf("\033[1;32m%s\033[0m\n", path); 
    }
}

void search_directory(const char *target_name, const char *dir_path, int only_files, int only_dirs, int only_exec, int execute_flag, const char *base_path, char *home_dir) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        printf("\033[1;31m"); 
        perror("opendir");
        printf("\033[0m");  
        return;
    }

    struct dirent *entry;
    char full_path[MAX_PATH_LENGTH];
    int found_count = 0;
    int is_dir_found = 0;
    int is_exec_found = 0;
    char found_path[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        struct stat path_stat;
        if (stat(full_path, &path_stat) != 0) {
            printf("\033[1;31m");  
            perror("stat");
            printf("\033[0m"); 
            continue;
        }

        int is_dir = S_ISDIR(path_stat.st_mode);
        int is_exec = (path_stat.st_mode & S_IXUSR) != 0; 
        int match = strncmp(entry->d_name, target_name, strlen(target_name)) == 0;

        if (match && ((!only_files && !only_dirs && !only_exec) || 
                      (only_files && !is_dir) || 
                      (only_dirs && is_dir) || 
                      (only_exec && is_exec))) {
            found_count++;
            strncpy(found_path, full_path, sizeof(found_path) - 1);
            found_path[sizeof(found_path) - 1] = '\0'; 

            if (is_dir) {
                is_dir_found = 1;
            } else if (is_exec) {
                is_exec_found = 1;
            }

            if (!execute_flag) {
                printf(" %s\n", full_path); 
                print_colored(full_path + strlen(base_path) + 1, is_dir);
            }
        }

        if (is_dir) {
            search_directory(target_name, full_path, only_files, only_dirs, only_exec, execute_flag, base_path, home_dir);
        }
    }

    closedir(dir);

    if (execute_flag) {
        if (found_count == 0) {
          
        } else {
            if (is_dir_found) {
                hop_to_directory(found_path, home_dir); 
            } else if (is_exec_found) {
                FILE *file = fopen(found_path, "r");
                if (file) {
                    char line[256];
                    while (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                    }
                    fclose(file);
                } else {
                    printf("\033[1;31m");  
                    perror("fopen");
                    printf("Missing permissions for task!\n");
                    printf("\033[0m"); 
                }
            } else {
                FILE *file = fopen(found_path, "r");
                if (file) {
                    char line[256];
                    while (fgets(line, sizeof(line), file)) {
                        printf("%s", line); 
                    }
                    fclose(file);
                } else {
                    printf("\033[1;31m");  
                    perror("fopen");
                    printf("Missing permissions for task!\n");
                    printf("\033[0m");  
                }
            }
        }
    }
}

void handle_seek_command(const char *input, char *home_dir) {
    int only_dirs = 0;
    int only_files = 0; 
    int only_exec = 0; 
    int execute_flag = 0; 
    
    const char *search_input = input;
    if (strncmp(input, "-d ", 3) == 0) {
        only_dirs = 1;
        search_input = input + 3; 
    } else if (strncmp(input, "-f ", 3) == 0) {
        only_files = 1;
        search_input = input + 3;
    } else if (strncmp(input, "-e ", 3) == 0) {
        only_exec = 1;
        execute_flag = 1; 
        search_input = input + 3; 
    }

    if (search_input[0] == ' ') {
        search_input++;
    }

    if (strlen(search_input) == 0) {
        printf("\033[1;31mError: No search term provided\033[0m\n");
        return;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("\033[1;31m");  
        perror("getcwd");
        printf("\033[0m");  
        return;
    }

    search_directory(search_input, cwd, only_files, only_dirs, only_exec, execute_flag, cwd, home_dir);
}
