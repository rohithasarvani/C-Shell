#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include "4.h"

#define MAX_PATH_LENGTH 4096
#define RED "\033[0;31m"
#define RESET "\033[0m"


const char *SHELL_HOME_DIR = "/home/rohitha/Documents/OSN_PROJECT-1"; 

void print_color(const char *path, mode_t mode) {
    if (S_ISDIR(mode)) {
        printf("\033[0;34m"); 
    } else if (mode & S_IXUSR) {
        printf("\033[0;32m"); 
    } else {
        printf("\033[0m"); 
    }
    printf("%s\033[0m", path);
}

void print_file_details(const char *path, const struct stat *st) {
    char time_buf[64];
    struct passwd *pwd;
    struct group *grp;

   
    printf((S_ISDIR(st->st_mode)) ? "d" : "-");
    printf((st->st_mode & S_IRUSR) ? "r" : "-");
    printf((st->st_mode & S_IWUSR) ? "w" : "-");
    printf((st->st_mode & S_IXUSR) ? "x" : "-");
    printf((st->st_mode & S_IRGRP) ? "r" : "-");
    printf((st->st_mode & S_IWGRP) ? "w" : "-");
    printf((st->st_mode & S_IXGRP) ? "x" : "-");
    printf((st->st_mode & S_IROTH) ? "r" : "-");
    printf((st->st_mode & S_IWOTH) ? "w" : "-");
    printf((st->st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    
    printf("%lu ", st->st_nlink);

    
    pwd = getpwuid(st->st_uid);
    grp = getgrgid(st->st_gid);
    printf("%s %s ", (pwd != NULL) ? pwd->pw_name : "unknown", (grp != NULL) ? grp->gr_name : "unknown");

    
    printf("%ld ", st->st_size);

    
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&st->st_mtime));
    printf("%s ", time_buf);

    
    print_color(path, st->st_mode);

    printf("\n");
}

char *expand_path(const char *path) {
    static char expanded_path[MAX_PATH_LENGTH];
    if (path[0] == '~') {
        if (path[1] == '\0') {
            
            snprintf(expanded_path, sizeof(expanded_path), "%s", SHELL_HOME_DIR);
        } else {
            
            snprintf(expanded_path, sizeof(expanded_path), "%s%s", SHELL_HOME_DIR, path + 1);
        }
    } else if (path[0] == '-') {
       
        snprintf(expanded_path, sizeof(expanded_path), "%s", path); 
    } else {
        snprintf(expanded_path, sizeof(expanded_path), "%s", path);
    }
    return expanded_path;
}

void reveal_directory(const char *dir_path, int show_all, int detailed) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[MAX_PATH_LENGTH];
    char resolved_path[MAX_PATH_LENGTH];
    size_t path_length;

   
    if (!realpath(dir_path, resolved_path)) {
        fprintf(stderr, RED "Error: Failed to resolve path %s: %s\n" RESET, dir_path, strerror(errno));
        return;
    }

    if (!(dir = opendir(resolved_path))) {
        fprintf(stderr, RED "Error: Failed to open directory %s: %s\n" RESET, resolved_path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }

       
        path_length = snprintf(path, sizeof(path), "%s/%s", resolved_path, entry->d_name);

        
        if (path_length >= sizeof(path)) {
            fprintf(stderr, RED "Warning: Path length exceeds buffer size and was truncated.\n" RESET);
            continue;
        }

        if (stat(path, &file_stat) == -1) {
            fprintf(stderr, RED "Error: Failed to stat file %s: %s\n" RESET, path, strerror(errno));
            continue;
        }

        if (detailed) {
            print_file_details(entry->d_name, &file_stat);
        } else {
            print_color(entry->d_name, file_stat.st_mode);
            printf("\n");
        }
    }

    closedir(dir);
}

void handle_reveal_command(char *input) {
    char *path = NULL;
    int show_all = 0;
    int detailed = 0;

   
    char *token = strtok(input, " ");
    
    
    while (token != NULL) {
        if (token[0] == '-') {
           
            if (strchr(token, 'a') != NULL) {
                show_all = 1;
            }
            if (strchr(token, 'l') != NULL) {
                detailed = 1;
            }
        } else {
           
            path = token;
        }
        token = strtok(NULL, " ");
    }

    if (path == NULL) {
        path = "."; 
    }

    char *expanded_path = expand_path(path);
    reveal_directory(expanded_path, show_all, detailed);
}
