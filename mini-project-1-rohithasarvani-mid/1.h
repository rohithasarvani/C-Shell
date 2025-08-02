#ifndef _1_H
#define _1_H

#include <stddef.h>

void get_username(char *username, size_t size);
void get_system_name(char *system_name, size_t size);
void get_current_directory(char *current_dir, size_t size);

void build_prompt(char *prompt, size_t size,char *home_dir);



#endif 
