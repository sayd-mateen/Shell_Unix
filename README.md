# Shell_Unix

This simple shell is a basic shell program that supports commands with I/O re-direction. 
Once, a user enters a command string (ended with a return key), your program should parse the command line and determines whether it has a regular command or contains I/O redirection signified by > for output to a file or < for input from a file. 
The program has also 3 built-in commands: cd (change directory), pwd (print working directory), and exit (exit shell). 
The built-in functions are not executed by forking and executing an executable. Instead, the shell process executes them itself. 
All other command must be executed in a child process. The shell is basically an interactive loop: it repeatedly prints a prompt "csc60msh > ", parses the input, executes the command specified on that line of input, and waits for the command to finish. 
I structured my shell such that it creates a new process for each new command (except for cd, pwd, and exit). Running commands in a new process protects the main shell process from any errors that occur in the new command.
