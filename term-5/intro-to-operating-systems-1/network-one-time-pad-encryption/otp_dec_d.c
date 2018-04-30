/******************************************************************************
 * FILE NAME:	otp_dec_d.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 4 - OTP
 * START DATE:	3/9/2018
 * DESCRIPTION: Acts as a server daemon which waits for otp_dec to send it the
 * 		contents of a cyphertext and key file. It parses the cyphertext
 * 		and key and uses them to decrypt the cyphertext, sending the
 * 		original message back to otp_dec.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <signal.h>

// Constants
#define CONTINUE 1
#define EXIT 0
#define BUFFER_SIZE 140100
#define END_TRANSMISSION '@'
#define HANDSHAKE_PROTOCOL "##"

// Error function used for reporting issues
void error(const char *msg) {perror(msg); exit(1);} 

// Converts character to its OTP encryption value
int getEncryptionValue(char character) {
	if ( (character >= 'A') && (character <= 'Z') ){
		return (character - 'A');
	}
	return 26;
}

// Converts OTP encryption value to a char
char getCharCode(int value) {
	if ( (value >= 0) && (value <= 25) ){
		return (value + 'A');
	}
	return ' ';
}

// performs modulo 27 subtraction with cyphertext and key values
int modulo27(int cypherInt, int keyInt){
	int result = cypherInt - keyInt;
	if (result <= -1){
		result += 27;
	}
	return result;
}

// Converts characters to OTP values, performs modulo 27 subtraction, and returns plain character
char getCipherChar(char cypherChar, char keyChar) {
	int cypherInt = getEncryptionValue(cypherChar);
	int keyInt = getEncryptionValue(keyChar);
	int result = modulo27(cypherInt, keyInt);
	return getCharCode(result);
}

// Parse transmission into cyphertext and key, decrypts cyphertext and stores it in buffer
void decrypt(char* transmission, char* plainBuffer){
	// Parse cyphertext from transmission
	char cyphertext[BUFFER_SIZE];
	memset(cyphertext, '\0', BUFFER_SIZE);
	int i = 0;
	while ( (i < strlen(transmission)) && (transmission[i] != '\n') ) {
		cyphertext[i] = transmission[i];
		i++;
	}

	i++; // Skip over '\n' between cyphertext and key

	// Parse key from transmission
	char key[BUFFER_SIZE];
	int j = 0;
	memset(key, '\0', BUFFER_SIZE);
	while ( (i < strlen(transmission)) && (transmission[i] != '\n') ) {
		key[j] = transmission[i];
		i++;
		j++;
	}

	// decrypt cyphertext character by character, writing result into buffer
	for (i = 0; i < strlen(cyphertext); ++i){
		plainBuffer[i] = getCipherChar(cyphertext[i], key[i]);
	}
}

int main(int argc, char *argv[]) {
	// Check usage & args
	if (argc < 2) {fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1);}

	// Ignore SIGCHLD to prevent child zombies
	struct sigaction ignoreAction = {0};
	ignoreAction.sa_handler = SIG_IGN;
	sigaction(SIGCHLD, &ignoreAction, NULL);

	// Set up the address struct for this process (the server)
	struct sockaddr_in serverAddress;
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	int portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	int listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("otp_dec_d: ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) error("otp_dec_d: ERROR on binding"); // Connect socket to port
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections



	// Infinite loop for parent process, but children will exit by changing flag
	int continueFlag = CONTINUE;
	while (continueFlag) {
		// Accept a connection, blocking if one is not available until one connects
		struct sockaddr_in clientAddress;
		socklen_t sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		int establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) {perror("otp_dec_d: ERROR on accept");}// report error, but daemon continues running
		else {

			// Fork process to recieve, work with, and return client data
			pid_t childPid = fork();
			if (childPid == -1) error("otp_dec_d: ERROR on fork"); // Error if fork fails

			// Fork is successful
			if (childPid == 0){
				
				// Declare transmission variables
				char buffer[BUFFER_SIZE]; memset(buffer, '\0', BUFFER_SIZE);
				char* offsetPtr = buffer;
				int bufferSize = strlen(HANDSHAKE_PROTOCOL);
				int charsRead;



				// Get handshake protocol from client
				while ((charsRead = recv(establishedConnectionFD, offsetPtr, bufferSize, 0)) > 0){
					offsetPtr += charsRead;
					bufferSize -= charsRead;
				}
				if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
				if (strcmp(HANDSHAKE_PROTOCOL, buffer) != 0){fprintf(stderr, "otp_dec_d: ERROR, client did not identify itself as otp_dec\n"); exit(1);}

				// Reset transmission variables for receiving
				memset(buffer, '\0', BUFFER_SIZE);
				offsetPtr = buffer;
				bufferSize = BUFFER_SIZE - 1;

				// Get cyphertext, key, and end transmission character from client
				while (((charsRead = recv(establishedConnectionFD, offsetPtr, bufferSize, 0)) > 0)
					&& (buffer[strlen(buffer) - 1] != END_TRANSMISSION)) {
					offsetPtr += charsRead;
					bufferSize -= charsRead;
				}
				if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
				if (buffer[strlen(buffer) - 1] != END_TRANSMISSION){fprintf(stderr, "otp_dec_d: ERROR, did not receive end transmisssion code"); exit(1);}
				
				// Parse cyphertext and key from transmission, decrypt message, and store it in plainBuffer
				char plainBuffer[BUFFER_SIZE];
				memset(plainBuffer, '\0', BUFFER_SIZE);
				decrypt(buffer, plainBuffer);


				// Append end transmission char to plaintext and declare variables for sending
				plainBuffer[strlen(plainBuffer)] = END_TRANSMISSION;
				int stringLength = strlen(plainBuffer);
				offsetPtr = plainBuffer;
				int charsWritten;

				// Send plaintext back to otp_dec
				while ((charsWritten = send(establishedConnectionFD, offsetPtr, stringLength, 0)) > 0){
					offsetPtr += charsWritten;
					stringLength -= charsWritten;
				}
				if ((stringLength > 0) || (charsWritten < 0)){fprintf(stderr, "otp_dec_d: ERROR writing to socket\n"); exit(1);}

				// Make sure all bytes are sent before closing socket
				int checkSend;
				do {ioctl(establishedConnectionFD, TIOCOUTQ, &checkSend);} while (checkSend > 0);
				if (checkSend < 0) error("otp_dec_d: ioctl error"); // Make sure loop wasn't exited due to error

				continueFlag = EXIT; // Set flag so that child does not continue loop
			}
			close(establishedConnectionFD); // Close the existing socket which is connected to the client
		}
	}
	close(listenSocketFD); // Close the listening socket
	return 0; 
}
