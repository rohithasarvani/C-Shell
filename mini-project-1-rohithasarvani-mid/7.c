#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

#define MAX_PATH 1024


#define STATUS_RUNNING 'R'
#define STATUS_SLEEPING 'S'
#define STATUS_ZOMBIE 'Z'
#define STATUS_FOREGROUND '+'
#define STATUS_BACKGROUND '-'


char get_process_status(pid_t pid) {
    char status = ' ';
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return status; 
    }
    
    char line[MAX_PATH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "State:", 6) == 0) {
            status = line[7]; 
            
            if (strchr(line, '+') != NULL) {
                status = (status == STATUS_RUNNING) ? STATUS_FOREGROUND :
                         (status == STATUS_SLEEPING) ? STATUS_FOREGROUND : status;
            } else if (strchr(line, '-') != NULL) {
                status = (status == STATUS_RUNNING) ? STATUS_BACKGROUND :
                         (status == STATUS_SLEEPING) ? STATUS_BACKGROUND : status;
            }
            break;
        }
    }
    fclose(file);
    
    return status;
}


long get_virtual_memory(pid_t pid) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1; 
    }
    
    long vm_size;
    if (fscanf(file, "%ld", &vm_size) != 1) {
        fclose(file);
        return -1; 
    }
    fclose(file);
    
    return vm_size * getpagesize() / 1024; 
}


void get_executable_path(pid_t pid, char *path, size_t size) {
    char exe_path[MAX_PATH];
    snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", pid);
    if (readlink(exe_path, path, size) == -1) {
        snprintf(path, size, "Unknown");
    } else {
        path[size - 1] = '\0'; 
    }
}


void proclore(int argc, char *argv[]) {
    pid_t pid;
    
    if (argc == 1) {
        pid = getpid(); 
    } else {
        char *endptr;
        pid = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0' || pid <= 0) {
            fprintf(stderr, "Invalid PID: %s\n", argv[1]);
            return;
        }
    }
    
    printf("pid : %d\n", pid);
    
  
    char status = get_process_status(pid);
    
  
    switch (status) {
        case STATUS_RUNNING:
            printf("process status : R\n");
            break;
        case STATUS_SLEEPING:
            printf("process status : S\n");
            break;
        case STATUS_ZOMBIE:
            printf("process status : Z\n");
            break;
        case STATUS_FOREGROUND:
            printf("process status : %c+\n", (status == STATUS_RUNNING) ? 'R' : 'S');
            break;
        case STATUS_BACKGROUND:
            printf("process status : %c-\n", (status == STATUS_RUNNING) ? 'R' : 'S');
            break;
        default:
            printf("process status : Unknown\n");
            break;
    }
    
  
    printf("Process Group : %d\n", getpgid(pid));
    
    
    long vm_size = get_virtual_memory(pid);
    if (vm_size >= 0) {
        printf("Virtual memory : %ld KB\n", vm_size);
    } else {
        printf("Virtual memory : Unknown\n");
    }
    
   
    char exe_path[MAX_PATH];
    get_executable_path(pid, exe_path, sizeof(exe_path));
    printf("Executable path : %s\n", exe_path);
}
