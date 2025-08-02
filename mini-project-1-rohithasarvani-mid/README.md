
1.c (to build the prompt)
get_username: Retrieves the current user's username using getpwuid and getuid, storing it in the provided buffer.
get_system_name: Retrieves the system's hostname using gethostname, storing it in the provided buffer.
get_current_directory: Retrieves the current working directory using getcwd, storing it in the provided buffer.
build_prompt: Constructs the shell prompt based on whether the current directory is within a specified home directory
home Directory Path: The assumption is that home_dir is the root of the home directory from which relative paths are computed.
Buffer Sizes: The code assumes that buffer sizes for usernames, system names, and current directory paths are sufficient for typical values.
Path Lengths: It assumes that directory paths will not exceed the allocated buffer sizes, such as 4096 for current_dir.

    If the current directory is within the home directory, it shows the prompt in the form <username@system:~relative_path>.
    Otherwise, it shows the full path <username@system:full_path>.
    2.c(command processing and handling of specification 2 and specification 6)
    trim_whitespace: Trims leading and trailing whitespace from a string.
execute_command:

    Logs the command.
    Parses the command into arguments.
    Forks a child process to execute the command using execvp.
    Measures and reports the time taken for execution if it exceeds 2 seconds.

execute_command_in_background:

    Logs the command.
    Parses the command into arguments.
    Forks a child process to execute the command in the background.
    Prints the PID of the background process and checks for its status.

process_commands:

    Splits the input string into separate commands using strtok_r.
    Handles various commands such as hop, reveal, log, seek, and proclore.
    For background processes, splits commands by & and executes them in the background.
    Executes regular commands using execute_command.
    Checks if fork fails by verifying if pid is -1.
If fork fails, prints an error message and exits with EXIT_FAILURE.
Measures the elapsed time of the command execution and prints a message if it exceeds 2 seconds.
Prints an error message if execvp fails to execute the command.
Continuously checks the status of background processes using waitpid with WNOHANG and prints messages about the process’s exit status.

(3.c hop )
hop_to_directory Function:

    Changes the current working directory based on the provided argument.
    Handles special cases like ~ for home, - for the previous directory, . for the current directory, and .. for the parent directory.
    Updates previous_dir to store the last directory before changing.

handle_hop_command Function:

    Parses the input command, trims whitespace, and calls hop_to_directory for each argument.
    Checks for failures in getcwd and chdir, printing error messages if operations fail.
    Ensures the constructed path does not exceed buffer limits.
    Assumes the provided directory paths are valid and accessible.
    (4.c reveal)
    Assumes buffer sizes (MAX_PATH_LENGTH)4096 are sufficient for file paths and names.

    Error: realpath fails to resolve the path.
    Handling: Prints error message with strerror(errno).

    Error: opendir fails to open the directory.
    Handling: Prints error message with strerror(errno).

    Warning: Path length exceeds buffer size.
    Handling: Prints warning message.

    Error: stat fails to retrieve file status.
    Handling: Prints error message with strerror(errno).

    Default Path: Uses . if no path is provided in the command.
    (5.c-log)
    Assumes command length (MAX_COMMAND_LENGTH)256 and log size (MAX_LOG_SIZE)15 are sufficient for typical usage.
    Error Opening Log File: fopen fails in save_log.

    Handling: Uses perror to print the error message.

Error Writing to Log File: fprintf fails in save_log.

    Handling: Uses perror to print the error message.

Error Flushing Log File: fflush fails in save_log.

    Handling: Uses perror to print the error message.

Error Closing Log File: fclose fails in save_log.

    Handling: Uses perror to print the error message.

get_log_file_path: Constructs the path for the log file based on the home directory.
load_log: Loads commands from the log file into memory.
save_log: Saves the current command log to the log file.
add_to_log: Adds a new command to the log and maintains log size constraints.
display_log: Prints the commands in the log with their indices.
clear_log: Clears the command log and updates the log file.
execute_log_entry: Executes a command from the log based on the provided index.
handle_log_command: Processes sub-commands for log management, including displaying, 

7.c(procure)
get_process_status: Retrieves the process status from /proc/[pid]/status, identifying if the process is running, sleeping, zombie, or its background/foreground status.

get_virtual_memory: Retrieves the virtual memory size of the process from /proc/[pid]/statm and converts it to kilobytes.

get_executable_path: Retrieves the executable path of the process from /proc/[pid]/exe.

proclore: Displays detailed information about a process, including its ID, status, process group, virtual memory size, and executable path. If no PID is provided, it uses the current process ID.
8.c[seek]
print_colored: Prints the provided path in blue if it's a directory and in green if it's a file, using ANSI escape codes for color formatting.

search_directory: Recursively searches through a directory for files or directories matching a target name, applying filters based on file type and executable status, and either prints the results or executes files based on flags.

handle_seek_command: Parses a command input to determine search parameters (directory, file, or executable), and calls search_directory to perform the search in the current working directory.