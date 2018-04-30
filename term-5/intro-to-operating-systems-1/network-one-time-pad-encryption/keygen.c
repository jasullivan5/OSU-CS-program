/******************************************************************************
 * FILE NAME:	keygen.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 4 - OTP
 * START DATE:	3/9/2018
 * DESCRIPTION:	Produces a key that can be used for one time pad encrypting
 * 		and decrypting. It accepts a command line argument for the
 * 		number of characters that the key should contain, then writes
 * 		a randomly generated key of uppercase letters and spaces to stdout.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Check to make sure keygen was called with key length argument
void validateArgumentCount(int argumentCount){
	if (argumentCount < 2){
		fprintf(stderr, "keygen: ERROR, no key length argument received\n");
		exit(1);
	}
}

// Check to make sure key length argument is valid
void validateKeyLength(int keyLength, char* argument){
	if (keyLength == 0){
		fprintf(stderr, "keygen: ERROR, '%s' is not a valid key length\n", argument);
		exit(1);
	}
}

// Generate a random integer within range of arguments
int getRandomIntInRange(int minInt, int maxInt){
	return (rand() % (maxInt + 1 - minInt) + minInt);
}

// Return a randomly generated uppercase letter or space
int getRandomUppercaseLetterOrSpace(void){
	int charCode = getRandomIntInRange('A' - 1, 'Z');
	if (charCode == ('A' - 1)){
		charCode = ' ';
	}
	return charCode;
}

// Assign specified number of random characters to a key buffer
void setKey(char* key, int keyLength){
	int i;
	for (i = 0; i < keyLength; ++i){
		key[i] = getRandomUppercaseLetterOrSpace();
	}
	key[i] = '\n';	
	key[i + 1] = '\0';
}

// Generate random key of length specified in command line argument
int main(int argc, char* argv[]){
	validateArgumentCount(argc);		// Make sure an argument was given
	srand(time(NULL));			// seed random number generation
	int keyLength = atoi(argv[1]);		// convert length of key from string to int
	validateKeyLength(keyLength, argv[1]);	// make sure keylength is an integer
	char key[keyLength + 2];		// key buffer (+2 for addition of \n and \0)
	setKey(key, keyLength);			// assign random letters or space to key
	printf(key); fflush(stdout);		// write key to stdout
	return 0;
}
