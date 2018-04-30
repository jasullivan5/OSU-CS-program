/******************************************************************************
 * FILE NAME:	smallsh.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 3 - smallsh
 * START DATE:	2/18/2018
 * DESCRIPTION: A basic shell program that prompts user for a command, expands "$$" 
 * 		within command into PID, parces command into individual arguments,
 * 		and attempts to execute	command. Has built-in commands for changing
 * 		directories, getting the exit status of the last foreground process,
 * 		and exiting the shell. All other commands are performed via a fork
 * 		and exec which attempts to find program to run in PATH. Processes
 * 		can be run in the background, and a foreground only mode can be
 * 		turned on and off. stdin and stdout can both be redirected.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include "StringArray.h"
#include "IntArray.h"

//Constants
#define EXIT 0
#define CONTINUE 1
#define BUFFER_SIZE 128
#define PID_SIZE 5
#define TOKEN_DELIMITERS " \n\t"
#define PID_SYMBOL "$$"
#define BACKGROUND_SYMBOL "&"
#define STDIN_SYMBOL "<"
#define STDOUT_SYMBOL ">"
#define COMMENT_SYMBOL '#'

//Function Prototypes
void shellLoop(struct sigaction* oldSIGINT_action);
void freeIfNotNull(char* command);
void catchSIGCHLD(int signalNumber);
void catchSIGTSTP(int signalNumber);
char* getCommand(int* inputFlag);
char* expandPID(char* string);
char* initializeExpandedString(char* string, int stringSize);
void getPidString(char* string); 
StringArray* parseArguments(char* command);
int execute(StringArray* argumentArray, char* childExitStatus, struct sigaction* oldSIGINT_action, IntArray* childPidArray);
void shExit(IntArray* childPidArray);
void cd(StringArray* argumentArray); 
void makeFork(StringArray* argumentArray, char* childExitStatus, struct sigaction* oldSIGINT_action, IntArray* childPidArray); 
int isBackgroundCommand(StringArray* argumentArray);
void setRedirections(StringArray* argumentArray, int backgroundFlag);
int setStdin(StringArray* argumentArray);
int setStdout(StringArray* argumentArray);
int getLowerInt(int int1, int int2);
void setExitStatus(int statusNumber, char* childExitStatus);
void setTerminatingSignal(int terminatingSignal, char* childExitStatus);

// Global flag for switching to/from foreground only mode in signal handler
int foregroundModeFlag = 0;



// The main function sets up signal handlers and starts the shell loop.
int main(void) {
	// Set up handler for ignoring SIGINT in shell
	struct sigaction newSIGINT_action = {0};
	struct sigaction oldSIGINT_action = {0};
	newSIGINT_action.sa_handler = SIG_IGN;
	sigaction(SIGINT, &newSIGINT_action, &oldSIGINT_action);

	// Set up handler for SIGTSTP foreground mode switch
	struct sigaction SIGTSTP_action = {0};
	SIGTSTP_action.sa_handler = catchSIGTSTP;
	sigfillset(&SIGTSTP_action.sa_mask);
	SIGTSTP_action.sa_flags = 0;
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);

	// Setup SIGCHLD handler for catching termination of background processes
	struct sigaction SIGCHLD_action	= {0};
	SIGCHLD_action.sa_handler = catchSIGCHLD;
	sigfillset(&SIGCHLD_action.sa_mask);
	SIGCHLD_action.sa_flags = 0;
	sigaction(SIGCHLD, &SIGCHLD_action, NULL);

	// Run shell loop
	shellLoop(&oldSIGINT_action);
	return 0;
}

// SIGTSTP handler switches global foregroundModeFlag and displays message
void catchSIGTSTP(int signalNumber) {
	if (foregroundModeFlag == 0) {
		foregroundModeFlag = 1;
		char* modeChar = "Entering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, modeChar, 49);
	}
	else if (foregroundModeFlag == 1) {
		foregroundModeFlag = 0;
		char* modeChar = "Exiting foreground-only mode\n";
		write(STDOUT_FILENO, modeChar, 29);
	}
}

// SIGCHLD handler waits for terminated background processes and prints their exit methods
void catchSIGCHLD(int signalNumber) {
	pid_t childPid;
	int childExitMethod;

	// Wait for all terminated background child processes and get exit method and PID of each
	while ((childPid = waitpid(-1, &childExitMethod, WNOHANG)) > 0) {
		int exitStatus;
		char statusMessage[BUFFER_SIZE] = {0};
		
		// Set message to be displayed if child process exited
		if (WIFEXITED(childExitMethod)) {
			exitStatus = WEXITSTATUS(childExitMethod);
			sprintf(statusMessage, "background pid %d is done: exit value %d\n", childPid, exitStatus);
		}
		
		// Set message t obe displayed if child process was interupted by signal
		if (WIFSIGNALED(childExitMethod)) {
			exitStatus = WTERMSIG(childExitMethod);
			sprintf(statusMessage, "background pid %d is done: terminated by signal %d\n", childPid, exitStatus);
		}
		
		// Print message to console
		int i = 0;
		while(statusMessage[i] != 0) {
			write(STDOUT_FILENO, &statusMessage[i], 1);
			i++;
		}

	}	
}



/***** SHELL LOOP *****/
// The shell loop allows handling of SIGCHLD signals before prompting the user for input.
// Input has $$ expanded to PID, is parsed, then sent to command execution functoin
// Finally, memory is freed and the loop repeats unless EXIT is returned from execution function.
void shellLoop(struct sigaction* oldSIGINT_action) {	
	// Block SIGCHLD. Unblocked later when we want to handle terminated background processes
	sigset_t SIGCHLD_set;
	sigemptyset(&SIGCHLD_set);
	sigaddset(&SIGCHLD_set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &SIGCHLD_set, NULL);

	// Shell loop gets command, expands $$ to PID, parses arguments, and executes commands until EXIT is returned
	int shExitFlag; // Flag for determining if shell loop should CONTINUE or EXIT
	IntArray* childPidArray = newIntArray(1); // Holds PIDs of all background processes
	char childExitStatus[BUFFER_SIZE] = {0}; // Holds exit status of last foreground process
	strcpy(childExitStatus, "No foreground child process terminated yet\n");	
	do {
		// Temporarily unblock SIGCHLD to handle any terminated background processes
		sigprocmask(SIG_UNBLOCK, &SIGCHLD_set, NULL);
		// Resume blocking SIGCHLD before user receives next prompt
		sigprocmask(SIG_BLOCK, &SIGCHLD_set, NULL);

		// Prompt user until input is received
		char* command = NULL; // Stores user input
		int inputFlag;
		do {
			printf(": "); fflush(stdout); // display prompt to user
			// Free memory and reset command ptr to NULL if getline was interupted
			freeIfNotNull(command);
			// Get command from user
			command = getCommand(&inputFlag);
			// Clear error in case of signal interuption
			clearerr(stdin); 
		} while (inputFlag == -1);

		// expand $$ to PID
		command = expandPID(command);
		// parse command
		StringArray* argumentArray = parseArguments(command);
		// Execute command
		shExitFlag = execute(argumentArray, childExitStatus, oldSIGINT_action, childPidArray);
		// Free memory
		freeStringArray(argumentArray);
		freeIfNotNull(command);
	} while (shExitFlag == CONTINUE);
}

// Free memory and set to pointer to NULL if it is not NULL
void freeIfNotNull(char* command) {
	if (command != NULL) {
		free(command);
		command = NULL;
	}
}

// Return pointer to command char and set inputFlag
char* getCommand(int* inputFlag) {
	char* command = NULL;
	size_t bufferSize = 0;
	*inputFlag = getline(&command, &bufferSize, stdin);
	return command;
}

// Replaces all instances of pid expansion symbol with the shell PID
char* expandPID(char* string) {
	// initialize expanded string	
	int stringSize = strlen(string) + 1; // +1 for \0
	char* expandedString = initializeExpandedString(string, stringSize);

	// get pid string
	char pid[PID_SIZE + 1];
	getPidString(pid);

	// Search string for expansion symbols and change them to PID
	int startIndex = 0;
	char* pidSymbolPointer;
	while ((pidSymbolPointer = strstr(&string[startIndex], PID_SYMBOL))) {	
		// resize expansion string so that PID fits	
		stringSize += (strlen(pid) - strlen(PID_SYMBOL));
		expandedString = realloc(expandedString, stringSize);
		assert(expandedString != NULL);
		// get size of substring
		int subStringSize = pidSymbolPointer - &string[startIndex];
		// add substring to expansion
		strncat(expandedString, &string[startIndex], subStringSize);
		// add PID to expansion
		strcat(expandedString, pid);
		// move start index passed $$ to beginning of next substring
		startIndex = (pidSymbolPointer - string) + strlen(PID_SYMBOL);
	}
	// add final substring to expanded string
	strcat(expandedString, &string[startIndex]);
	// free old string and return expanded string
	free(string);
	return expandedString;
}

// Allocate memory and zero out buffer for expanded string
char* initializeExpandedString(char* string, int stringSize) {
	char* expandedString = malloc(stringSize * sizeof(char));
	assert(expandedString != NULL);
	memset(expandedString, '\0', stringSize * sizeof(char));
	return expandedString;
}

// Get PID and turn into string
void getPidString(char* string) {
	int pid = getpid();
	sprintf(string, "%d", pid);
}

// Divde command into array of individual argument strings
StringArray* parseArguments(char* command) {
	StringArray* tokenArray = newStringArray(1);
	char* token = strtok(command, TOKEN_DELIMITERS);
	while (token != NULL) {
			addStringArray(tokenArray, token);
			token = strtok(NULL, TOKEN_DELIMITERS);
	}
	return tokenArray;
}



/*****COMMAND EXECUTION*****/
// Execute returns to prompt if comment or no arguments are entered.
// If first argument is built-in command, then appropriate built-in is run.
// All other commands proceed to the makeFork function.
int execute(StringArray* argumentArray, char* childExitStatus, struct sigaction* oldSIGINT_action, IntArray* childPidArray) {
	if(argumentArray->size > 0) {
		// If command is a comment, continue to next prompt
		if (argumentArray->data[0][0] == COMMENT_SYMBOL) {
			return CONTINUE;
		}
		// Clean up and exit shell
		if (strcmp(argumentArray->data[0], "exit") == 0) {
			shExit(childPidArray);
			return EXIT;
		}
		// Run cd built-in
		if (strcmp(argumentArray->data[0], "cd") == 0) {
			cd(argumentArray);
			return CONTINUE;
		}
		// Run status built-in
		if (strcmp(argumentArray->data[0], "status") == 0) {
			printf(childExitStatus);
			return CONTINUE;
		}
		// For all other commands, fork process
		makeFork(argumentArray, childExitStatus, oldSIGINT_action, childPidArray);
		return CONTINUE;		
	}
	return CONTINUE; // Continue to next prompt if no arguments were entered
}

// Exit built-in: Terminates any remaining background processes, reaps zombies, and frees PID array
void shExit(IntArray* childPidArray) {
	int i;
	for (i = 0; i < childPidArray->size; ++i) {
		while (waitpid(childPidArray->data[i], NULL, WNOHANG) == 0) {
			kill(childPidArray->data[i], SIGTERM);
			waitpid(childPidArray->data[i], NULL, WNOHANG);
		}
	}
	freeIntArray(childPidArray);
}

// Changes the directory to HOME directory with no arguments, to directory specified in first argument,
// or prints an error if first argument is not a valid directory.
void cd(StringArray* argumentArray) {
	// if multiple arguments and argument 2 is not '&'
	if ((argumentArray->size > 1) && (strcmp(argumentArray->data[1], BACKGROUND_SYMBOL) != 0)) {
		if ((chdir(argumentArray->data[1])) != 0) {
			printf("smallsh: cd: No such directory\n"); fflush(stdout);
		}
		return;
	}
	chdir(getenv("HOME"));
}



/*****FORK/EXEC*****/
// Checks if process should be run in background, forks the process, sets redirections, and passes arguments to exec within fork.
// Waits for foreground process and stores exit status, or announces background process to user and stores PID in array.
void makeFork(StringArray* argumentArray, char* childExitStatus, struct sigaction* oldSIGINT_action, IntArray* childPidArray) {
	// Set background flag if command ends in background symbol and foreground only mode is not set
	int backgroundFlag = isBackgroundCommand(argumentArray);
	
	// Fork process	
	pid_t childPid;
	pid_t childExitMethod;
	childPid = fork();
	switch (childPid) {
		case -1: // Print erro if fork failed
			perror("smallsh: makeFork: fork failure");
			exit(1);
			break;

		case 0: // If fork succeeds
			// set input and output redirections
			setRedirections(argumentArray, backgroundFlag);
			
			// Remove block on SIGINT for foreground process
			if (backgroundFlag == 0) {
				sigaction(SIGINT, oldSIGINT_action, NULL);
			}
			
			// Pass arguments to exec to start external program
			if (execvp(argumentArray->data[0], argumentArray->data) < 0) {
				// Print error and exit upon exec failure
				perror("smallsh: makeFork: exec failure");
				exit(1);
			}
			break;

		default: // Proceed with main shell process
			if (backgroundFlag == 0) {
				int testPid;
				do { // Wait for foreground process until it has completed
					testPid = waitpid(childPid, &childExitMethod, 0);
				} while (testPid == -1);

				// Get exit method and set exit status of foreground process
				if (WIFEXITED(childExitMethod)) {
					setExitStatus(WEXITSTATUS(childExitMethod), childExitStatus);
				}
				if (WIFSIGNALED(childExitMethod)) {
					setTerminatingSignal(WTERMSIG(childExitMethod), childExitStatus);
				}
			}

			else { // If background process, display PID and add it to background child PID array
				printf("background pid is %d\n", childPid); fflush(stdout);
				addIntArray(childPidArray, childPid);
			}
	}
}

// If multiple arguments and last is background symbol, remove it and set background flag if mode allows
int isBackgroundCommand(StringArray* argumentArray) {
	// If more than 1 argument and last argument is Background symbol
	if ((argumentArray->size > 1) && (strcmp(argumentArray->data[argumentArray->size - 1], BACKGROUND_SYMBOL) == 0)) {
		// Replace Background symbol with NULL pointer
		argumentArray->size--;
		argumentArray->data[argumentArray->size] = NULL;
		// Sets background process flag only if foregroundModeFlag is not set
		if (foregroundModeFlag == 0) {
			return 1;
		}
	}
	return 0;
}

// If command has more than two arguments and stdin/stdout symbols are found, attempt to redirect input and output to
// arguments provided. If background flag is set and no stdin/stdout symbols were found redirect to /dev/null
// Remove any redirectoins arguments from argumentArray before sending to exec
void setRedirections(StringArray* argumentArray, int backgroundFlag) {
	// Initialize stdin and stdout symbol indeces to that of NULL pointer at end of argument array
	int stdinSymbolIndex = argumentArray->size;
	int stdoutSymbolIndex = argumentArray->size;

	// If there are more than two arguments, attempt to set redirections if redirection symbols are present
	if (argumentArray->size > 2) {
		stdinSymbolIndex = setStdin(argumentArray);
		stdoutSymbolIndex = setStdout(argumentArray);
		// Find lowest index of redirection symbols and set pointer to NULL 
		// to exclude all redirection arguemnts from set of arguments sent to exec
		int lowerIndex = getLowerInt(stdinSymbolIndex, stdoutSymbolIndex);
		argumentArray->data[lowerIndex] = NULL;
	}

	// If background process... 
	if (backgroundFlag == 1) {
		// and no stdin symbol found, set stdin to /dev/null
		if (stdinSymbolIndex == argumentArray->size) {
			int source = open("/dev/null", O_RDONLY);
			if (source == -1) {
				perror("smallsh: SetRedirections: failed to open /dev/null");
				exit(1);
			}
			if (dup2(source, 0) == -1) {
				perror("smallsh: setRedirections: failed to redirect stdin");
				exit(1);
			}
		}
		// and no stdout symbol found, set stdin to /dev/null
		if (stdoutSymbolIndex == argumentArray->size) {
			int target = open("/dev/null", O_WRONLY);
			if (target == -1) {
				perror("smallsh: setRedirections: failed to open /dev/null");
				exit(1);
			}

			if (dup2(target, 1) == -1) {
				perror("smallsh: setRedirections: failed to redirect stdout");
				exit(1);
			}
		}
	}
}

// Search for stdin symbol. If found, attempt to redirect stdin to argument after symbol.
// Prints errors if opening of file descriptor or redirection fail.
// return index of symbol if found, else return array size
int setStdin(StringArray* argumentArray) {
	// Search argument array for stdin symbol
	int i;
	for (i = 1; i < argumentArray->size - 1; ++i) {
		// if stdin symbol is found...
		if (strcmp(argumentArray->data[i], STDIN_SYMBOL) == 0) {
			// Attempt to open and redirect stdin to argument after symbol, else print error message
			int source = open(argumentArray->data[i + 1], O_RDONLY);
			if (source == -1) {
				perror("smallsh: setStdin: failed to open file descriptor");
				exit(1);
			}

			if (dup2(source, 0) == -1) {
				perror("smallsh: setStdin: failed to redirect stdin");
				exit(1);
			}
			return i; // Return index of symbol
		}
	}
	return argumentArray->size; // return array size if symbol isn't found
}

// Search for stdout symbol. If found, attempt to redirect stdout to argument after symbol.
// Prints errors if opening of file descriptor or redirection fail.
// return index of symbol if found, else return array size
int setStdout(StringArray* argumentArray) {
	// Search argument array for stdout symbol
	int i;
	for (i = 1; i < argumentArray->size - 1; ++i) {
		// if stdout symbol is found...
		if (strcmp(argumentArray->data[i], STDOUT_SYMBOL) == 0) {
			// Attempt to open and redirect stdout to argument after symbol, else print error message
			int target = open(argumentArray->data[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (target == -1) {
				perror("smallsh: setStdout: failed to open file descriptor");
				exit(1);
			}

			if (dup2(target, 1) == -1) {
				perror("smallsh: setStdout: failed to redirect stdout");
				exit(1);
			}
			return i; // Return index of symbol
		}
	}
	return argumentArray->size; // return array size if symbol isn't found
}

// Return the lower of two integers
int getLowerInt(int int1, int int2) {
	if (int1 <= int2) {
		return int1;
	}
	return int2;
}

// Set child exit status when child process has terminated by itself
void setExitStatus(int statusNumber, char* childExitStatus) {
	sprintf(childExitStatus, "exit value %d\n", statusNumber);
}

// Set child exit status when child process has terminated via signal
// and prints message immediately
void setTerminatingSignal(int terminatingSignal, char* childExitStatus) {
	sprintf(childExitStatus, "terminated by signal %d\n", terminatingSignal);
	printf(childExitStatus);
}
