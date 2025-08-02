// 5.h
#ifndef _5_H
#define _5_H

void get_log_file_path(const char *home_dir, char *log_file_path);
void load_log(const char *home_dir);
void save_log(const char *home_dir);
void add_to_log(const char *command, const char *home_dir);
void display_log(void);
void clear_log(const char *home_dir);
void execute_log_entry(int index, char *home_dir); 
void handle_log_command(char *input, const char *home_dir);

#endif
