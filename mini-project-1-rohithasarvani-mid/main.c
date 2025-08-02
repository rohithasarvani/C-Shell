#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "1.h"
#include "2.h"
#include "3.h"
#include "5.h"

int main() {
    char prompt[4096 + 100];
    char command[1024];
    char home_dir[4096]; 
    get_current_directory(home_dir, sizeof(home_dir));

   
    load_log(home_dir);

    while (1) {
        
        build_prompt(prompt, sizeof(prompt), home_dir);
        printf("%s", prompt);

       
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; 
        }

       
        command[strcspn(command, "\n")] = 0;

        
        process_commands(command, home_dir);
    }

    return 0;
}
