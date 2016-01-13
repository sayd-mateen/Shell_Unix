# Shell_Unix

This simple shell is a basic shell program that supports commands with I/O re-direction. 
Once, a user enters a command string (ended with a return key), your program should parse the command line and determines whether it has a regular command or contains I/O redirection signified by > for output to a file or < for input from a file. 
The program has also 3 built-in commands: cd (change directory), pwd (print working directory), and exit (exit shell). 
The built-in functions are not executed by forking and executing an executable. Instead, the shell process executes them itself. 
All other command must be executed in a child process. The shell is basically an interactive loop: it repeatedly prints a prompt "csc60msh > ", parses the input, executes the command specified on that line of input, and waits for the command to finish. 
I structured my shell such that it creates a new process for each new command (except for cd, pwd, and exit). Running commands in a new process protects the main shell process from any errors that occur in the new command.


# For Shell_Two
- Added features

The shell can redirect stdout to a file. The shell can also directly attach one process's stdout to another's stdin using something called a 'pipe'. A pipe is a one-way channel in the kernel: at one end you can only write, at the other you can only read. To a process, a pipe looks more or less like a file descriptor, and by attaching the 'write' end of a pipe to one process's stdout and the 'read' end to another's stdin, it is possible to feed the output of one process directly to the input of another. If the user types "program1 args1 | program2 args2", use the output from program1 as the input to program2. These features are relatively easy to implement. After fork (but before exec), the STDIN and STDOUT file descriptors are already set up to refer to user-typed input and output to the terminal respectively. The dup2 system call is useful for setting STDIN and STDOUT to refer to other sources/destinations of data. The pipe system call (man 2 pipe) may be useful for setting up a pair of file descriptors for piping.

