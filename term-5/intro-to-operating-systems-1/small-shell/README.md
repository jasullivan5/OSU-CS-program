The smallsh program acts as a shell that provides the user with a command prompt. It has the built in
commands "exit" which exits the smallsh program, "cd" which changes the current directory,
and "status" which displays the exit status or terminating signal of the last foreground process. Bash
commands can also be performed. This is handled by use of fork() and exec(). Processes can also be started
in the background by ending the command with a "&" argument. Background processes can be disabled by hitting
Ctrl-Z. Ctrl-C terminates any process running in the foreground, but does not exit the shell. redirection of stdin and stdout
with the < and > operators. A makefile is included with the program, so you can just type "make" to compile the smallsh. Then type "smallsh" to run the program.
