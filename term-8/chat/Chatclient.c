/*********************************************************************
** Program Filename: Chatclient.c
** Author: Joseph Sullivan, ONID: sullijos
** Class: CS 372 Intro to Computer Networks
** Date last modified: 02/10/2019
** Tested on: flip3.engr.oregonstate.edu
** Description: Implements a chat client over a TCP connection.
**              Ensures that at least two command line arguments were passed for
**              server hostname and listening port. Reqeusts and stores server
**              address info. Connects to server. Gets input for user handle.
**              Continually alternate between sending and recieving messages
**              to and from server as long as user hasn't typed "\quit" and
**              connection has not been closed. Closes socket and frees memory
**              before exiting program.
*********************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOST_NAME 1
#define PORT 2
#define NUM_ARGS 3
#define PROMPT_LENGTH 2
#define MAX_HANDLE_LENGTH 10
#define MAX_MESSAGE_LENGTH 500

/*********************************************************************
** Function: void error(char *message)
** Description: Calls perror and exits program with EXIT_FAILURE.
** Parameters: char *message: Error message passed to perror.
*********************************************************************/
void error(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}


/*********************************************************************
** Function: void getSrverInfo(struct addrinfo **serverInfo, char **argv)
** Description: Sets parameters for desired address family and socket type.
                Requests info of host indicated by the hostname and port
                indicated in command line arguments. Prints error and exits
                upon failure to obtain info.
** Parameters: struct addrinfo **serverInfo: struct to populate with requested
               address info.
               char **argv: command line arguments for hostname and port.
*********************************************************************/
void getServerInfo(struct addrinfo **serverInfo, char **argv)
{
    // Set parameters for server address info retrieved by getaddrinfo()
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Get server info
    int result = getaddrinfo(argv[HOST_NAME], argv[PORT], &hints, serverInfo);

    // Print error and exit upon failure to obtain server info
    if (result != 0)
    {
        fprintf(stderr, "ERROR: Failed to obtain server info\n");
        exit(EXIT_FAILURE);
    }
}


/*********************************************************************
** Function: int connectToServer(struct addrinfo *serverInfo)
** Description: Tries connecting to server with each set of address info results
                stored in serverInfo's linked list. If all connections fail, an
                error is printed and program is exited. Success returns socket
                file descriptor of connection.
** Parameters: struct addrinfo *serverInfo: struct containing linked list of
               host address info results obtained in getServerInfo().
** Return: Returns integer socket file descriptor of connection to server.
*********************************************************************/
int connectToServer(struct addrinfo *serverInfo)
{
    struct addrinfo *addrinfoPtr = serverInfo;
    int socketFD;
    int connectResult;

    // Try all server address info results in linked list
    while (addrinfoPtr != NULL)
    {
        // Get socket file descriptor
        socketFD = socket(addrinfoPtr->ai_family, addrinfoPtr->ai_socktype, addrinfoPtr->ai_protocol);
        if (socketFD != -1)
        {
            // Connect with server
            connectResult = connect(socketFD, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
            if (connectResult != -1)
            {
                break; // Exit while loop if connection succeeds
            }
        }
        // If connection fails, close socket and try next address info result in list
        close(socketFD);
        addrinfoPtr = addrinfoPtr->ai_next;
    }

    // If all results fail print errror and exit
    if (addrinfoPtr == NULL)
    {
        freeaddrinfo(serverInfo);
        error("ERROR: Failed to connect to server");
    }
    return socketFD; // Otherwise, return socket file descriptor for connection
}

/*********************************************************************
** Function: void getHandle(char *handle, size_t handleSize)
** Description: Prompt user for input of their handle until they enter
                a handle that is between 1 and 10 chars and contains no spaces.
                Also removes new line character from end of handle.
** Parameters: char *handle: string for holding user handle.
               size_t handleSize: Size of handle array in bytes.
*********************************************************************/
void getHandle(char *handle, size_t handleSize)
{
    char ch;
    do // Prompt user for handle until input is between 1 and 10 chars and contains no spaces
    {
        // Get handle input from user
        printf("Please enter your handle: ");
        fgets(handle, handleSize, stdin);

        // remove new line from handle
        if (handle[strlen(handle) - 1] == '\n')
        {
            handle[strlen(handle) - 1] = '\0';
        }

        // Check if handle is between 1 and 10 chars and has no spaces
        if (strcspn(" ", handle) == 0 || strlen(handle) > MAX_HANDLE_LENGTH || strlen(handle) < 1)
        {

            // If input was greater than maximum handle size, clear input buffer
            if (strlen(handle) > MAX_HANDLE_LENGTH)
            {
                while ((ch = getchar()) != '\n' && ch != EOF) { }
            }

            // Alert user of handle requirements
            printf("Handle must be between 1 and 10 characters and cannot contain spaces\n");
        }
    }while (strcspn(" ", handle) == 0 || strlen(handle) > MAX_HANDLE_LENGTH || strlen(handle) < 1);
}


/*********************************************************************
** Function: void inputMessage(char *input, size_t inputSize, char *handle)
** Description: Prompts user for message input until user inputs message
                of 500 or fewer characters.
** Parameters: char *input: Message buffer for user input
               size_t inputSize: size of input buffer in bytes
               char *handle: user's handle
*********************************************************************/
void inputMessage(char *input, size_t inputSize, char *handle)
{
    // Create prompt, appending user handle and prompt symbol to string
    char prompt[MAX_HANDLE_LENGTH + PROMPT_LENGTH + 1]; // +1 for \0
    memset(prompt, '\0', sizeof(prompt));
    strcat(prompt, handle);
    strcat(prompt, "> ");

    char ch;
    do // Prompt user for message until length requirement is met
    {
        // print prompt for user input and get message from user
        printf("%s", prompt);
        fgets(input, inputSize, stdin);

        // if newline character gets cut off, input was too long
        if (input[strlen(input) - 1] != '\n')
        {
            // clear input buffer and alert user of message length requirements
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Message must be 500 or fewer characters\n");
        }
    }while (input[strlen(input) - 1] != '\n');
}


/*********************************************************************
** Function: void sendMessage(int socketFD, char *handle, char *input)
** Description: Construct message using handle, prompt symbol and user input.
                Repeatedly send to server until all chars are sent. Print error
                and exit if send fails.
** Parameters: int socketFD: socket to send message to.
               char *handle: user's handle.
               char *input: message to send, input by user.
*********************************************************************/
void sendMessage(int socketFD, char *handle, char *input)
{
    // Construct message by concatenating handle, prompt symbol, and input
    char message[MAX_HANDLE_LENGTH + MAX_MESSAGE_LENGTH + PROMPT_LENGTH + 2]; // +2 for \n and \0
    memset(message, '\0', sizeof(message));
    strcat(message, handle);
    strcat(message, "> ");
    strcat(message, input);

    // Try sending until all message characters are sent.
    int charsSent = 0;
    int totalCharsSent = 0;
    while (charsSent < strlen(message))
    {
        charsSent = send(socketFD, &message[totalCharsSent], strlen(message), 0);
        // Print error and exit if send fails
        if (charsSent < 0)
        {
            error("ERROR: Failed to send to socket");
        }
        // Add chars sent from last call to send to number of total chars sent
        totalCharsSent += charsSent;
    }
}


/*********************************************************************
** Function: size_t receiveMessage(int socketFD)
** Description: Receives bytes from server until '\0' is recieved and stores
                message in buffer. If call to recv returns empty message,
                alerts user that connection was closed. If receive fails,
                prints error, and exits program.
** Parameters: int socketFD: Socket for receiving message.
** Return: Returns size_t length of message string recieved.
*********************************************************************/
size_t receiveMessage(int socketFD)
{
    int messageLength = MAX_MESSAGE_LENGTH + MAX_HANDLE_LENGTH + PROMPT_LENGTH + 2; // +2 for \n and \0
    char message[messageLength];
    memset(message, '\0', messageLength);
    int charsReceived = 0;
    int totalCharsReceived = 0;

    do // Received bytes until '\0' is received
    {
        charsReceived = recv(socketFD, &message[totalCharsReceived], messageLength, 0);
        // If recv fails, print error and exit program
        if(charsReceived < 0)
        {
            error("ERROR: Failed to read from socket");
        }
        // Add chars recieved from last call to recv to total chars recieved
        totalCharsReceived += charsReceived;
    } while(message[charsReceived] != '\0');

    // If recv returned with no message, alert user that connection was closed
    if (strlen(message) == 0)
    {
        printf("Server closed connection\n");
    }
    else // Otherwise, print the returned message
    {
        printf("%s", message);
    }
    // Return length of message received.
    return strlen(message);
}


/*********************************************************************
** Function: int main(int argc, char **argv)
** Description: Ensures that at least two command line arguments were passed.
                Reqeusts and stores server address info. Connects to server.
                Gets input for user handle. Continually alternate between sending
                and recieving messages to and from server as long as user hasn't
                typed "\quit" and connection has not been closed. Closes socket
                and frees memory before exiting program.
** Parameters: int argc: number of command line arguments
               char **argv: array of command line argument strings
** Return: Returns exit value of 0
*********************************************************************/
int main(int argc, char **argv)
{
    // Make sure at least two command line arguments were passed
    // If not, print error and exit
    if (argc < NUM_ARGS)
    {
        fprintf(stderr, "USAGE: Chatclient hostname port\n");
        exit(EXIT_FAILURE);
    }

    // Request and store server address info
    struct addrinfo *serverInfo;
    getServerInfo(&serverInfo, argv);
    printf("Obtained server info\n");

    // Connect to server
    printf("Trying to connect to server...\n");
    int socketFD = connectToServer(serverInfo);
    printf("Connected to server\n");

    // Get input for user handle
    char handle[MAX_HANDLE_LENGTH + 2]; // +2 for \n and \0
    getHandle(handle, sizeof(handle));

    // Alternate between sending and recieving messages until user enters "\quit"
    // or connection is closed by server.
    char input[MAX_MESSAGE_LENGTH + 2]; // +2 for \n and \0
    size_t receivedLength;
    do
    {
        // Get user's message to send to server
        inputMessage(input, sizeof(input), handle);

        // If user hasn't quit, send a message, and then wait to receive reply
        if (strcmp(input,"\\quit\n") != 0)
        {
            sendMessage(socketFD, handle, input);
            receivedLength = receiveMessage(socketFD);
        }
    }while(strcmp(input,"\\quit\n") != 0 && receivedLength != 0);

    // Close socket and free memory before exiting program
    close(socketFD);
    freeaddrinfo(serverInfo);
    printf("Socket closed\nExiting chat\n");
    return 0;
}
