/******************************************************************************
 * FILE NAME:	otp_dec.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 4 - OTP
 * START DATE:	3/9/2018
 * DESCRIPTION: Sends a cyphertext and key for one time pad decryption to a
 * 		daemon-like server program that performs the decryption. Then,
 * 		receives the decrypted text from the daemon and writes it to stdout.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>

// Constants
#define BUFFER_SIZE 140100
#define END_TRANSMISSION '@'
#define HANDSHAKE_PROTOCOL "##"


// Read characters from file into buffer.
// If any invalid characters are found report an error
void getFileText(char* fileName, char* buffer) {
	// open file for reading
	FILE* file = fopen(fileName, "r");
	if (file == NULL) { fprintf(stderr, "otp_dec: ERROR, failed to open file\n"); exit(1); }

	// Read all characters into buffer as long as no invalid characters are found
	int charsRead = 0;
	int character;
	while ( (charsRead < (BUFFER_SIZE - 1)) && ((character = fgetc(file)) != EOF) ) {
		if ( ((character >= 'A') && (character <= 'Z')) || (character == ' ') || (character == '\n') ){
			buffer[charsRead] = character;
			charsRead++;
		}
		else { fprintf(stderr, "otp_dec: ERROR, invalid characters detected in '%s'\n", fileName); exit(1); }
	}
	fclose(file);
}

int main(int argc, char *argv[]) {
	if (argc < 4) { fprintf(stderr, "USAGE: %s cyphertext key port\n", argv[0]); exit(1); } // Check usage & args
	
	// get cyphertext into buffer
	char cyphertext[BUFFER_SIZE];
	memset(cyphertext, '\0', BUFFER_SIZE);
	getFileText(argv[1], cyphertext);

	// get key into buffer
	char key[BUFFER_SIZE];
	memset(key, '\0', BUFFER_SIZE);
	getFileText(argv[2], key);
       	
	// exit if key is too short
	if (strlen(key) < strlen(cyphertext)) { fprintf(stderr, "otp_dec: ERROR, key must be at least as long as cyphertext"); exit(1); }

	// Set up the server address struct	
	struct sockaddr_in serverAddress;
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	int portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	struct hostent* serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "otp_dec: ERROR, no such host on port: %d\n", portNumber); exit(2); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	int socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) { fprintf(stderr, "otp_dec: ERROR opening socket on port: %d\n", portNumber); exit(2); }
	
	// Connect to server, by connecting socket to address
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) { fprintf(stderr, "otp_dec: ERROR connecting on port: %d\n", portNumber); exit(2); }



	// Send handshake protocol to confirm for server that connecting process is otp_dec
	int stringLength = strlen(HANDSHAKE_PROTOCOL);
	char* offsetPtr = HANDSHAKE_PROTOCOL;
	int charsWritten;
	// Keep sending until all characters have been sent
	while ((charsWritten = send(socketFD, offsetPtr, stringLength, 0)) > 0){
		offsetPtr += charsWritten;
		stringLength -= charsWritten;
	}
	// Check if error forced loop to exit before all characters were sent
	if ((stringLength > 0) || (charsWritten < 0)) { fprintf(stderr, "otp_dec: ERROR writing to socket on port: %d\n", portNumber); exit(2); }



	// Load cyphertext, key, and end transmission character into buffer
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	strcat(buffer, cyphertext);
	strcat(buffer, key);
	buffer[strlen(buffer)] = END_TRANSMISSION;



	// send cyphertext, key, and end transmission character to otp_dec_d
	stringLength = strlen(buffer);
	offsetPtr = buffer;
	while ((charsWritten = send(socketFD, offsetPtr, stringLength, 0)) > 0){
		offsetPtr += charsWritten;
		stringLength -= charsWritten;
	}
	if ((stringLength > 0) || (charsWritten < 0)) { fprintf(stderr, "otp_dec: ERROR writing to socket on port: %d\n", portNumber); exit(2); }

	// Make sure that all bytes have been sent before continuing
	int checkSend;
	do {ioctl(socketFD, TIOCOUTQ, &checkSend); } while (checkSend > 0);
	if (checkSend < 0) { fprintf(stderr, "otp_dec: ioctl error on port %d\n", portNumber); exit(2); } // Make sure loop wasn't exited due to error



	// Get decrypted text back from otp_dec_d
	memset(buffer, '\0', BUFFER_SIZE); // Clear out the buffer again for reuse
	offsetPtr = buffer;
	int bufferSize = BUFFER_SIZE - 1; // save 1 byte for '\0'
	int charsRead;
	// While no errors occur, request bytes from socket until end transmission character has been received.
	while (((charsRead = recv(socketFD, offsetPtr, bufferSize, 0)) > 0)
		&& (buffer[strlen(buffer) - 1] != END_TRANSMISSION)) { 
		offsetPtr += charsRead;
		bufferSize -= charsRead;
	}

	if (charsRead < 0) { fprintf(stderr, "otp_dec: ERROR reading from socket on port: %d\n", portNumber); exit(2); }
	if (buffer[strlen(buffer) - 1] != END_TRANSMISSION) { fprintf(stderr, "otp_dec: ERROR, did not receive end transmission code on port: %d\n", portNumber); exit(2); }
	buffer[strlen(buffer) - 1] = '\n'; // Replace end transmission character with newline character in decrypted text
	printf("%s", buffer); // write decryted text to stdout

	close(socketFD); // Close the socket
	return 0;
}
