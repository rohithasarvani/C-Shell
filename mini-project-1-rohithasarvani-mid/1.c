#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include "1.h"

#define PURPLE "\033[0;35m"
#define RESET "\033[0m"

void get_username(char *username, size_t size) {
    struct passwd *pw = getpwuid(getuid());
    strncpy(username, pw->pw_name, size - 1);
    username[size - 1] = '\0';
}

void get_system_name(char *system_name, size_t size) {
    gethostname(system_name, size - 1);
    system_name[size - 1] = '\0';
}

void get_current_directory(char *current_dir, size_t size) {
    getcwd(current_dir, size);
}

void build_prompt(char *prompt, size_t size, char *home_dir) {
    char username[32], system_name[32], current_dir[4096];

    get_username(username, sizeof(username));
    get_system_name(system_name, sizeof(system_name));
    get_current_directory(current_dir, sizeof(current_dir));

    if (strncmp(current_dir, home_dir, strlen(home_dir)) == 0) {
        snprintf(prompt, size, PURPLE "<%s@%s:~%s> " RESET, username, system_name, current_dir + strlen(home_dir));
    } else {
        snprintf(prompt, size, PURPLE "<%s@%s:%s> " RESET, username, system_name, current_dir);
    }
}
